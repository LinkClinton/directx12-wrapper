#include "device.hpp"

#ifndef _NDEBUG
#define __ENABLE_DIRECTX_DEBUG_LAYER__
#endif

wrapper::directx12::device::device(const ComPtr<ID3D12Device5>& source) : mDevice(source)
{
}

ID3D12Device5* const* wrapper::directx12::device::get_address_off() const
{
	return mDevice.GetAddressOf();
}

ID3D12Device5* wrapper::directx12::device::operator->() const
{
	return mDevice.Get();
}

ID3D12Device5* wrapper::directx12::device::get() const
{
	return mDevice.Get();
}

wrapper::directx12::device wrapper::directx12::device::create(const D3D_FEATURE_LEVEL& level)
{
#ifdef __ENABLE_DIRECTX_DEBUG_LAYER__
	ComPtr<ID3D12Debug> debug;

	D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

	debug->EnableDebugLayer();
#endif

	ComPtr<ID3D12Device5> source;

	D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(source.GetAddressOf()));

	return device(source);
}
