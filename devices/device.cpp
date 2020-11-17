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

void wrapper::directx12::device::create_depth_stencil_view(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc,
	const D3D12_CPU_DESCRIPTOR_HANDLE& handle, const resource& resource) const
{
	mWrapperInstance->CreateDepthStencilView(resource.get(), &desc, handle);
}

void wrapper::directx12::device::create_shader_resource_view(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc,
                                                             const D3D12_CPU_DESCRIPTOR_HANDLE& handle, const resource& resource) const
{
	mWrapperInstance->CreateShaderResourceView(resource.get(), &desc, handle);
}

void wrapper::directx12::device::create_shader_resource_view(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc,
	const D3D12_CPU_DESCRIPTOR_HANDLE& handle) const
{
	// When you create SRV on Raytracing Acceleration, the pResource should be null
	mWrapperInstance->CreateShaderResourceView(nullptr, &desc, handle);
}

void wrapper::directx12::device::create_constant_buffer_view(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc,
                                                             const D3D12_CPU_DESCRIPTOR_HANDLE& handle) const
{
	mWrapperInstance->CreateConstantBufferView(&desc, handle);
}

void wrapper::directx12::device::create_unordered_access_view(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc,
	const D3D12_CPU_DESCRIPTOR_HANDLE& handle, const resource& resource) const
{
	mWrapperInstance->CreateUnorderedAccessView(resource.get(), nullptr, &desc, handle);
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
