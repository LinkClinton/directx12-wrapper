#include "command_list.hpp"

#include "../resources/buffer.hpp"

wrapper::directx12::graphics_command_list::graphics_command_list(const ComPtr<ID3D12GraphicsCommandList4>& source)
	: wrapper_t<ID3D12GraphicsCommandList4>(source)
{
}

void wrapper::directx12::graphics_command_list::clear_unordered_access_view(
	const D3D12_CPU_DESCRIPTOR_HANDLE& cpu_handle_in_non_shader_visible_heap, 
	const D3D12_GPU_DESCRIPTOR_HANDLE& gpu_handle_in_shader_visible_heap,
	const resource& resource, const std::array<float, 4>& value) const
{
	mWrapperInstance->ClearUnorderedAccessViewFloat(
		gpu_handle_in_shader_visible_heap, 
		cpu_handle_in_non_shader_visible_heap, 
		resource.get(), value.data(), 0, nullptr);
}

void wrapper::directx12::graphics_command_list::clear_render_target_view(const D3D12_CPU_DESCRIPTOR_HANDLE& view,
                                                                         const std::array<float, 4>& color) const
{
	mWrapperInstance->ClearRenderTargetView(view, color.data(), 0, nullptr);
}

void wrapper::directx12::graphics_command_list::clear_depth_stencil_view(const D3D12_CPU_DESCRIPTOR_HANDLE& view,
	float depth, uint8 stencil) const
{
	mWrapperInstance->ClearDepthStencilView(view, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
		depth, stencil, 0, nullptr);
}

void wrapper::directx12::graphics_command_list::set_descriptor_heaps(
	const std::vector<ComPtr<ID3D12DescriptorHeap>>& heaps) const
{
	std::vector<ID3D12DescriptorHeap*> origins(heaps.size());

	std::transform(heaps.begin(), heaps.end(), origins.begin(),
		[](const ComPtr<ID3D12DescriptorHeap> heap) { return heap.Get(); });
	
	mWrapperInstance->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), origins.data());
}

void wrapper::directx12::graphics_command_list::set_render_targets(
	const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& views, const D3D12_CPU_DESCRIPTOR_HANDLE& depth_stencil) const
{
	mWrapperInstance->OMSetRenderTargets(static_cast<UINT>(views.size()), views.data(), false, &depth_stencil);
}

void wrapper::directx12::graphics_command_list::set_render_targets(
	const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& views) const
{
	mWrapperInstance->OMSetRenderTargets(static_cast<UINT>(views.size()), views.data(), false, nullptr);
}

void wrapper::directx12::graphics_command_list::set_vertex_buffers(const std::vector<D3D12_VERTEX_BUFFER_VIEW>& views, size_t start_slot) const
{
	mWrapperInstance->IASetVertexBuffers(static_cast<UINT>(start_slot), static_cast<UINT>(views.size()), views.data());
}

void wrapper::directx12::graphics_command_list::set_index_buffer(const D3D12_INDEX_BUFFER_VIEW& view) const
{
	mWrapperInstance->IASetIndexBuffer(&view);
}

void wrapper::directx12::graphics_command_list::set_view_ports(const std::vector<D3D12_VIEWPORT>& viewports) const
{
	mWrapperInstance->RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
}

void wrapper::directx12::graphics_command_list::set_scissor_rects(const std::vector<D3D12_RECT>& rects) const
{
	mWrapperInstance->RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
}

void wrapper::directx12::graphics_command_list::set_compute_descriptor_table(uint32 index,
	const D3D12_GPU_DESCRIPTOR_HANDLE& base) const
{
	mWrapperInstance->SetComputeRootDescriptorTable(index, base);
}

void wrapper::directx12::graphics_command_list::set_graphics_descriptor_table(uint32 index,
                                                                              const D3D12_GPU_DESCRIPTOR_HANDLE& base) const
{
	mWrapperInstance->SetGraphicsRootDescriptorTable(index, base);
}

void wrapper::directx12::graphics_command_list::set_graphics_shader_resource_view(uint32 index,
                                                                                  const resource& resource) const
{
	mWrapperInstance->SetGraphicsRootShaderResourceView(index, resource->GetGPUVirtualAddress());
}

void wrapper::directx12::graphics_command_list::set_graphics_constant_buffer_view(uint32 index,
                                                                                  const buffer& resource) const
{
	mWrapperInstance->SetGraphicsRootConstantBufferView(index, resource->GetGPUVirtualAddress());
}

void wrapper::directx12::graphics_command_list::set_graphics_constants(uint32 index, const void* data, uint32 count, uint32 offset) const
{
	mWrapperInstance->SetGraphicsRoot32BitConstants(index, count, data, offset);
}

void wrapper::directx12::graphics_command_list::set_graphics_constants(uint32 index, const std::vector<value32bit>& values) const
{
	set_graphics_constants(index, values.data(), static_cast<uint32>(values.size()), 0);
}

void wrapper::directx12::graphics_command_list::resource_barrier(
	const std::vector<D3D12_RESOURCE_BARRIER>& barriers) const
{
	mWrapperInstance->ResourceBarrier(static_cast<UINT>(barriers.size()), barriers.data());
}

void wrapper::directx12::graphics_command_list::close() const
{
	mWrapperInstance->Close();
}

wrapper::directx12::graphics_command_list wrapper::directx12::graphics_command_list::create(const device& device, const command_allocator& allocator)
{
	ComPtr<ID3D12GraphicsCommandList4> list;

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.get(), nullptr,
		IID_PPV_ARGS(list.GetAddressOf()));

	list->Close();
	
	return graphics_command_list(list);
}
