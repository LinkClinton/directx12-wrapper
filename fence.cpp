#include "fence.hpp"

wrapper::directx12::fence::fence(const ComPtr<ID3D12Fence>& source) : wrapper_t<ID3D12Fence>(source)
{
}

wrapper::directx12::fence wrapper::directx12::fence::create(const device& device, size_t value)
{
	ComPtr<ID3D12Fence> fence;

	device->CreateFence(static_cast<UINT64>(value), D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));

	return directx12::fence(fence);
}
