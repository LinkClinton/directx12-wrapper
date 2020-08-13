#include "triangle_application.hpp"

#include "../../directx12-wrapper/extensions/imgui.hpp"
#include "../../directx12-wrapper/extensions/dxc.hpp"

wrapper::samples::triangle_application::triangle_application(const std::string& name, int width, int height) :
	application(name, width, height)
{
	mDevice = directx12::device::create(D3D_FEATURE_LEVEL_12_0);

	mFence = directx12::fence::create(mDevice, 0);
	
	mCommandAllocator = directx12::command_allocator::create(mDevice);
	mCommandQueue = directx12::command_queue::create(mDevice);

	mCommandList = directx12::graphics_command_list::create(mDevice, mCommandAllocator);
	
	mSwapChain = directx12::swap_chain::create(mCommandQueue, width, height, mHandle);

	mRenderTargetViewHeap = directx12::descriptor_heap::create(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, mSwapChain.buffers().size());

	for (size_t index = 0; index < mSwapChain.buffers().size(); index++) {
		D3D12_RENDER_TARGET_VIEW_DESC desc;

		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Format = mSwapChain.format();
		desc.Texture2D.MipSlice = 0;
		desc.Texture2D.PlaneSlice = 0;

		mDevice->CreateRenderTargetView(mSwapChain.buffers()[index].get(), &desc, mRenderTargetViewHeap.cpu_handle(index));
	}
	
	mInputAssemblyInfo.add_input_element("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	
	mDepthStencilInfo.set_depth_enable(false);
	
	mRootSignatureInfo.add_constants("config", 0, 0, 6);
	
	mRootSignature = directx12::root_signature::create(mDevice, mRootSignatureInfo);

	mVertShader = directx12::extensions::compile_from_file_using_dxc(L"./shaders/main_shader.hlsl", L"vs_main", L"vs_6_0");
	mFragShader = directx12::extensions::compile_from_file_using_dxc(L"./shaders/main_shader.hlsl", L"ps_main", L"ps_6_0");
	
	mGraphicsPipelineInfo
		.set_primitive_type(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
		.set_input_assembly(mInputAssemblyInfo)
		.set_rasterization(mRasterizationInfo)
		.set_depth_stencil(mDepthStencilInfo)
		.set_blend(mBlendInfo)
		.set_root_signature(mRootSignature)
		.set_vert_shader(mVertShader)
		.set_frag_shader(mFragShader)
		.set_format({ mSwapChain.format() });

	mGraphicsPipeline = directx12::pipeline_state::create(mDevice, mGraphicsPipelineInfo);

	mVertexBuffer = directx12::buffer::create(mDevice, directx12::resource_info::upload(), sizeof(float) * 9);

	std::vector<float> vertices = {
		width / 2.0f, height / 2.0f + height * 0.1f, 0,
		width / 2.0f + width * 0.1f, height / 2.0f, 0,
		width / 2.0f - width * 0.1f, height / 2.0f, 0
	};

	mVertexBuffer.copy_from_cpu(vertices.data(), sizeof(float) * vertices.size());

	mImGuiDescriptorHeap = directx12::descriptor_heap::create(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	
	directx12::extensions::imgui_context::initialize(
		mDevice, mImGuiDescriptorHeap,
		mImGuiDescriptorHeap.cpu_handle(), mImGuiDescriptorHeap.gpu_handle(),
		mSwapChain.format(), 2);
}

void wrapper::samples::triangle_application::update(float delta)
{
	directx12::extensions::imgui_context::new_frame();
	ImGui::NewFrame();

	ImGui::Begin("triangle_sample");
	ImGui::ColorEdit4("Color", mColor.data());
	ImGui::End();
}

void wrapper::samples::triangle_application::render(float delta)
{
	float color[4] = { 1, 1, 1, 1 };

	const auto current_frame_index = mSwapChain->GetCurrentBackBufferIndex();
	const auto render_target_view = mRenderTargetViewHeap.cpu_handle(current_frame_index);
	
	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.get(), nullptr);

	mSwapChain.buffers()[current_frame_index].barrier(mCommandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	mCommandList.clear_render_target_view(render_target_view, { 1, 1, 1, 1 });
	mCommandList.set_render_targets({ render_target_view });

	mCommandList->SetGraphicsRootSignature(mRootSignature.get());
	mCommandList->SetPipelineState(mGraphicsPipeline.get());
	
	mCommandList.set_vertex_buffers({
		directx12::resource_view::vertex_buffer(mVertexBuffer, sizeof(float) * 3, sizeof(float) * 9)
		});

	mCommandList.set_view_ports({
		{ 0, 0, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.f, 1.f }
		});

	mCommandList.set_scissor_rects({
		{ 0, 0, static_cast<LONG>(mWidth), static_cast<LONG>(mHeight) }
		});

	mCommandList.set_graphics_constants(
		mRootSignatureInfo.index("config"), 
		{
		mColor[0], mColor[1], mColor[2], mColor[3],
		static_cast<directx12::uint32>(mWidth),
		static_cast<directx12::uint32>(mHeight)
		});
	
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->DrawInstanced(3, 1, 0, 0);

	mCommandList.set_descriptor_heaps({ mImGuiDescriptorHeap.get() });
	
	ImGui::Render();
	directx12::extensions::imgui_context::render(mCommandList, ImGui::GetDrawData());
	
	mSwapChain.buffers()[current_frame_index].barrier(mCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	mCommandList->Close();
	mCommandQueue.execute({ mCommandList });

	mSwapChain.present();
	
	mCommandQueue.wait(mFence);
}
