#include "command_allocator.hpp"

wrapper::directx12::command_allocator::command_allocator(const ComPtr<ID3D12CommandAllocator>& source) : wrapper_t<ID3D12CommandAllocator>(source)
{
}

wrapper::directx12::command_allocator wrapper::directx12::command_allocator::create(const device& device)
{
	ComPtr<ID3D12CommandAllocator> allocator;

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.GetAddressOf()));

	return command_allocator(allocator);
}
