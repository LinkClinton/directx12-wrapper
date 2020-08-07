#include "command_list.hpp"

wrapper::directx12::graphics_command_list::graphics_command_list(const ComPtr<ID3D12GraphicsCommandList4>& source) : mCommandList(source)
{
}

ID3D12GraphicsCommandList4* const* wrapper::directx12::graphics_command_list::get_address_off() const
{
	return mCommandList.GetAddressOf();
}

ID3D12GraphicsCommandList4* wrapper::directx12::graphics_command_list::operator->() const
{
	return mCommandList.Get();
}

ID3D12GraphicsCommandList4* wrapper::directx12::graphics_command_list::get() const
{
	return mCommandList.Get();
}

wrapper::directx12::graphics_command_list wrapper::directx12::graphics_command_list::create(const device& device, const command_allocator& allocator)
{
	ComPtr<ID3D12GraphicsCommandList4> list;

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.get(), nullptr,
		IID_PPV_ARGS(list.GetAddressOf()));

	return graphics_command_list(list);
}
