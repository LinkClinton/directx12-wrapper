#include "command_allocator.hpp"

wrapper::directx12::command_allocator::command_allocator(const ComPtr<ID3D12CommandAllocator>& source) : mAllocator(source)
{
}

ID3D12CommandAllocator* const* wrapper::directx12::command_allocator::get_address_off() const
{
	return mAllocator.GetAddressOf();
}

ID3D12CommandAllocator* wrapper::directx12::command_allocator::operator->() const
{
	return mAllocator.Get();
}

ID3D12CommandAllocator* wrapper::directx12::command_allocator::get() const
{
	return mAllocator.Get();
}

wrapper::directx12::command_allocator wrapper::directx12::command_allocator::create(const device& device)
{
	ComPtr<ID3D12CommandAllocator> allocator;

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.GetAddressOf()));

	return command_allocator(allocator);
}
