#include "descriptor_heap.hpp"

wrapper::directx12::descriptor_heap::descriptor_heap(const ComPtr<ID3D12DescriptorHeap>& source, const ComPtr<ID3D12Device5>& device)
	: wrapper_t<ID3D12DescriptorHeap>(source)
{
	const auto desc = mWrapperInstance->GetDesc();

	mOffset = device->GetDescriptorHandleIncrementSize(desc.Type);
	mCount = static_cast<size_t>(desc.NumDescriptors);
}

D3D12_CPU_DESCRIPTOR_HANDLE wrapper::directx12::descriptor_heap::cpu_handle(size_t index) const
{
	assert(index < mCount);
	
	return offset_handle(mWrapperInstance->GetCPUDescriptorHandleForHeapStart(), index * mOffset);
}

D3D12_GPU_DESCRIPTOR_HANDLE wrapper::directx12::descriptor_heap::gpu_handle(size_t index) const
{
	assert(index < mCount);

	return offset_handle(mWrapperInstance->GetGPUDescriptorHandleForHeapStart(), index * mOffset);
}

wrapper::directx12::descriptor_heap wrapper::directx12::descriptor_heap::create(const device& device,
	const D3D12_DESCRIPTOR_HEAP_TYPE& type, size_t count)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;

	desc.NodeMask = 0;
	desc.Flags = type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = type;
	desc.NumDescriptors = static_cast<UINT>(count);

	ComPtr<ID3D12DescriptorHeap> heap;
	
	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heap.GetAddressOf()));

	return descriptor_heap(heap, device.get());
}
