#include "device.hpp"

#include "../resources/resource.hpp"

#ifndef _NDEBUG
#define __ENABLE_DIRECTX_DEBUG_LAYER__
#endif

wrapper::directx12::device::device(const ComPtr<ID3D12Device5>& source) : wrapper_t<ID3D12Device5>(source)
{
}

void wrapper::directx12::device::create_render_target_view(const D3D12_RENDER_TARGET_VIEW_DESC& desc,
	const D3D12_CPU_DESCRIPTOR_HANDLE& handle, const resource& resource) const
{
	mWrapperInstance->CreateRenderTargetView(resource.get(), &desc, handle);
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

wrapper::directx12::device wrapper::directx12::device::create(const adapter& adapter, const D3D_FEATURE_LEVEL& level)
{
#ifdef __ENABLE_DIRECTX_DEBUG_LAYER__
	ComPtr<ID3D12Debug> debug;

	D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

	debug->EnableDebugLayer();
#endif

	ComPtr<ID3D12Device5> source;

	D3D12CreateDevice(adapter.get(), level, IID_PPV_ARGS(source.GetAddressOf()));

	return device(source);
}
