#pragma once

#define __DIRECTX_WRAPPER_EXTENSION_IMGUI__
#ifndef __DIRECTX_WRAPPER_EXTENSION_IMGUI__
#else
#include "../descriptors/root_signature.hpp"
#include "../pipelines/pipeline_state.hpp"
#include "../commands/command_queue.hpp"
#include "../resources/texture2d.hpp"
#include "../resources/buffer.hpp"

#include <imgui.h>

namespace wrapper::directx12::extensions {

	class imgui_frame_resources {
	public:
		explicit imgui_frame_resources(const device& device);

		~imgui_frame_resources() = default;

		void update(ImDrawData* draw_data);

		buffer vtx_buffer() const;

		buffer idx_buffer() const;
	private:
		static inline const size_t VtxBufferIncrease = 500;
		static inline const size_t IdxBufferIncrease = 5000;

		device mDevice;

		buffer mVtxBuffer;
		buffer mIdxBuffer;

		size_t mCurrentVtxBufferCount = 0;
		size_t mCurrentIdxBufferCount = 0;
	};
	
	class imgui_context final {
	public:
		imgui_context() = delete;

		~imgui_context() = delete;

		static void initialize(
			const device& device, const descriptor_heap& heap,
			const D3D12_CPU_DESCRIPTOR_HANDLE& font_cpu_handle,
			const D3D12_GPU_DESCRIPTOR_HANDLE& font_gpu_handle,
			const DXGI_FORMAT& render_target_view_format,
			size_t num_frames_in_flight);

		static void new_frame();

		static void set_render_state(
			const graphics_command_list& command_list,
			const imgui_frame_resources& resources, 
			ImDrawData* draw_data);

		static void set_multi_sample(size_t samples);
		
		static void render(const graphics_command_list& command_list, ImDrawData* draw_data);
	private:
		static void create_pipeline_state();

		static void create_font_texture();
	private:
		static inline device Device;

		static inline root_signature_info RootSignatureInfo;
		static inline root_signature RootSignature;

		static inline input_assembly_info InputAssemblyInfo;
		static inline rasterization_info RasterizationInfo;
		static inline depth_stencil_info DepthStencilInfo;
		static inline blend_info BlendInfo;

		static inline graphics_pipeline_info GraphicsPipelineInfo;
		static inline pipeline_state GraphicsPipeline;

		static inline descriptor_table DescriptorTable;
		static inline descriptor_heap DescriptorHeap;

		static inline shader_code VertShader;
		static inline shader_code FragShader;
		
		static inline texture2d FontTexture;
		
		static inline std::vector<imgui_frame_resources> FrameResources;

		static inline D3D12_CPU_DESCRIPTOR_HANDLE FontCpuHandle;
		static inline D3D12_GPU_DESCRIPTOR_HANDLE FontGpuHandle;
		static inline DXGI_FORMAT RenderTargetViewFormat;
		
		static inline size_t CurrentFrameIndex = 0;
	};

	inline imgui_frame_resources::imgui_frame_resources(const device& device) : mDevice(device)
	{
		mVtxBuffer = buffer::create(
			mDevice,
			resource_info::upload(),
			VtxBufferIncrease * sizeof(ImDrawVert));

		mIdxBuffer = buffer::create(
			mDevice,
			resource_info::upload(),
			IdxBufferIncrease * sizeof(ImDrawIdx));

		mCurrentVtxBufferCount = VtxBufferIncrease;
		mCurrentIdxBufferCount = IdxBufferIncrease;
	}

	inline void imgui_frame_resources::update(ImDrawData* draw_data)
	{
		const auto old_vtx_buffer_count = mCurrentVtxBufferCount;
		const auto old_idx_buffer_count = mCurrentIdxBufferCount;
		
		while (mCurrentVtxBufferCount < draw_data->TotalVtxCount) mCurrentVtxBufferCount += VtxBufferIncrease;
		while (mCurrentIdxBufferCount < draw_data->TotalIdxCount) mCurrentIdxBufferCount += IdxBufferIncrease;

		if (mCurrentVtxBufferCount > old_vtx_buffer_count)
			mVtxBuffer = buffer::create(
				mDevice,
				resource_info::upload(),
				mCurrentVtxBufferCount * sizeof(ImDrawVert)
			);

		if (mCurrentIdxBufferCount > old_idx_buffer_count)
			mIdxBuffer = buffer::create(
				mDevice,
				resource_info::upload(),
				mCurrentIdxBufferCount * sizeof(ImDrawIdx)
			);

		auto vtx_memory = static_cast<ImDrawVert*>(mVtxBuffer.begin_mapping());
		auto idx_memory = static_cast<ImDrawIdx*>(mIdxBuffer.begin_mapping());
		
		for (auto index = 0; index < draw_data->CmdListsCount; index++) {
			const auto command_list = draw_data->CmdLists[index];

			std::memcpy(vtx_memory, command_list->VtxBuffer.Data, command_list->VtxBuffer.size_in_bytes());
			std::memcpy(idx_memory, command_list->IdxBuffer.Data, command_list->IdxBuffer.size_in_bytes());

			vtx_memory = vtx_memory + command_list->VtxBuffer.Size;
			idx_memory = idx_memory + command_list->IdxBuffer.Size;
		}

		mVtxBuffer.end_mapping();
		mIdxBuffer.end_mapping();
	}

	inline buffer imgui_frame_resources::vtx_buffer() const
	{
		return mVtxBuffer;
	}

	inline buffer imgui_frame_resources::idx_buffer() const
	{
		return mIdxBuffer;
	}

	inline void imgui_context::initialize(const device& device, const descriptor_heap& heap,
	                                      const D3D12_CPU_DESCRIPTOR_HANDLE& font_cpu_handle, 
	                                      const D3D12_GPU_DESCRIPTOR_HANDLE& font_gpu_handle,
	                                      const DXGI_FORMAT& render_target_view_format, 
	                                      size_t num_frames_in_flight)
	{
		auto& imgui_io = ImGui::GetIO();

		imgui_io.BackendRendererName = "imgui_directx12_wrapper";
		imgui_io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		Device = device;
		RenderTargetViewFormat = render_target_view_format;
		DescriptorHeap = heap;
		FontCpuHandle = font_cpu_handle;
		FontGpuHandle = font_gpu_handle;

		CurrentFrameIndex = 0;

		for (size_t index = 0; index < num_frames_in_flight; index++)
			FrameResources.push_back(imgui_frame_resources(Device));

		create_pipeline_state();
	}

	inline void imgui_context::new_frame()
	{
		if (!GraphicsPipeline.get()) create_pipeline_state();
	}

	inline void imgui_context::set_render_state(
		const graphics_command_list& command_list,
		const imgui_frame_resources& resources, ImDrawData* draw_data)
	{
		//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L68
		const auto L = draw_data->DisplayPos.x;
		const auto R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		const auto T = draw_data->DisplayPos.y;
		const auto B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

		float mvp[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
		};

		D3D12_VIEWPORT viewport = {
			0, 0,
			draw_data->DisplaySize.x,
			draw_data->DisplaySize.y,
			0.f, 1.0f
		};

		D3D12_VERTEX_BUFFER_VIEW vtx_view = {
			resources.vtx_buffer()->GetGPUVirtualAddress(),
			static_cast<UINT>(resources.vtx_buffer().size_in_bytes()),
			static_cast<UINT>(sizeof(ImDrawVert))
		};

		D3D12_INDEX_BUFFER_VIEW idx_view = {
			resources.idx_buffer()->GetGPUVirtualAddress(),
			static_cast<UINT>(resources.idx_buffer().size_in_bytes()),
			sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT
		};
		
		command_list->RSSetViewports(1, &viewport);

		command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		command_list->IASetVertexBuffers(0, 1, &vtx_view);
		command_list->IASetIndexBuffer(&idx_view);

		command_list->SetPipelineState(GraphicsPipeline.get());
		command_list->SetGraphicsRootSignature(RootSignature.get());
		command_list->SetGraphicsRoot32BitConstants(
			static_cast<UINT>(RootSignatureInfo.index("view")),
			16, mvp, 0
		);

		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		command_list->OMSetBlendFactor(blend_factor);
	}

	inline void imgui_context::set_multi_sample(size_t samples)
	{
		GraphicsPipelineInfo.set_multi_sample(samples);

		GraphicsPipeline = pipeline_state::create(Device, GraphicsPipelineInfo);
	}

	inline void imgui_context::render(const graphics_command_list& command_list, ImDrawData* draw_data)
	{
		//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L123
		// Avoid rendering when minimized
		if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
			return;

		auto& frameResources = FrameResources[CurrentFrameIndex];

		frameResources.update(draw_data);

		set_render_state(command_list, frameResources, draw_data);

		size_t global_vtx_offset = 0;
		size_t global_idx_offset = 0;

		const auto clipOff = draw_data->DisplayPos;

		for (int nCommandList = 0; nCommandList < draw_data->CmdListsCount; nCommandList++) {
			const ImDrawList* commandList = draw_data->CmdLists[nCommandList];

			for (int nCommand = 0; nCommand < commandList->CmdBuffer.Size; nCommand++) {
				const auto command = &commandList->CmdBuffer[nCommand];

				if (command->UserCallback != nullptr) {
					if (command->UserCallback == ImDrawCallback_ResetRenderState)
						set_render_state(command_list, frameResources, draw_data);
					else
						command->UserCallback(commandList, command);
				}
				else {
					//draw call commands
					D3D12_RECT rect = {
						static_cast<LONG>(command->ClipRect.x - clipOff.x),
						static_cast<LONG>(command->ClipRect.y - clipOff.y),
						static_cast<LONG>(command->ClipRect.z - clipOff.x),
						static_cast<LONG>(command->ClipRect.w - clipOff.y)
					};

					command_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootSignatureInfo.index("table")), 
						*(D3D12_GPU_DESCRIPTOR_HANDLE*)&command->TextureId);
					
					command_list->RSSetScissorRects(1, &rect);

					command_list->DrawIndexedInstanced(command->ElemCount, 1,
						static_cast<UINT>(command->IdxOffset + global_idx_offset),
						static_cast<UINT>(command->VtxOffset + global_vtx_offset), 0);
				}
			}

			global_idx_offset = global_idx_offset + commandList->IdxBuffer.Size;
			global_vtx_offset = global_vtx_offset + commandList->VtxBuffer.Size;
		}

		CurrentFrameIndex = (CurrentFrameIndex + 1) % FrameResources.size();
	}

	inline void imgui_context::create_pipeline_state()
	{
		DescriptorTable = descriptor_table();

		DescriptorTable.add_srv_range({ "texture" }, 0, 0);

		RootSignatureInfo = root_signature_info();

		RootSignatureInfo
			.add_descriptor_table("table", DescriptorTable)
			.add_constants("view", 0, 0, 16)
			.add_static_sampler("sampler", 0, 0);

		RootSignature = root_signature::create(Device, RootSignatureInfo);

		static const char* vert_shader =
			"cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

		static const char* frag_shader =
			"struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            SamplerState sampler0 : register(s0);\
            Texture2D texture0 : register(t0);\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";

		VertShader = shader_code::create_from_source(vert_shader, "main", "vs_5_0");
		FragShader = shader_code::create_from_source(frag_shader, "main", "ps_5_0");
		
		InputAssemblyInfo
			.add_input_element("POSITION", DXGI_FORMAT_R32G32_FLOAT)
			.add_input_element("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT)
			.add_input_element("COLOR", DXGI_FORMAT_R8G8B8A8_UNORM);

		RasterizationInfo
			.set_fill_mode(D3D12_FILL_MODE_SOLID)
			.set_cull_mode(D3D12_CULL_MODE_NONE);
		
		DepthStencilInfo.set_depth_enable(false);

		BlendInfo.set_render_target({
			true,
			false,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		}, 0);

		GraphicsPipelineInfo
			.set_primitive_type(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
			.set_root_signature(RootSignature)
			.set_vert_shader(VertShader)
			.set_frag_shader(FragShader)
			.set_input_assembly(InputAssemblyInfo)
			.set_rasterization(RasterizationInfo)
			.set_depth_stencil(DepthStencilInfo)
			.set_blend(BlendInfo)
			.set_format({ RenderTargetViewFormat });

		GraphicsPipeline = pipeline_state::create(Device, GraphicsPipelineInfo);

		create_font_texture();
	}

	inline void imgui_context::create_font_texture()
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		FontTexture = texture2d::create(Device,
			resource_info::common(D3D12_RESOURCE_STATE_COPY_DEST),
			DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

		const auto pitch = FontTexture.alignment();
		const auto size = FontTexture.height() * pitch;

		const auto upload = buffer::create(Device, resource_info::upload(), size);

		const auto data = static_cast<byte*>(upload.begin_mapping());

		for (size_t index = 0; index < FontTexture.height(); index++)
			std::memcpy(data + index * pitch, pixels + index * width * 4, static_cast<size_t>(width) * 4);
		
		upload.end_mapping();

		const auto allocator = command_allocator::create(Device);
		const auto queue = command_queue::create(Device);
		const auto command_list = graphics_command_list::create(Device, allocator);
		const auto fence = fence::create(Device, 0);

		allocator->Reset();
		command_list->Reset(allocator.get(), nullptr);

		FontTexture.copy_from(command_list, upload);
		FontTexture.barrier(command_list, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

		command_list->Close();
		
		queue.execute({ command_list });
		queue.wait(fence);

		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};

		desc.Format = FontTexture.format();
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		
		Device->CreateShaderResourceView(FontTexture.get(), &desc, FontCpuHandle);

		io.Fonts->TexID = reinterpret_cast<ImTextureID>(FontGpuHandle.ptr);
	}

}

#endif