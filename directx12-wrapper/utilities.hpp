#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#undef max
#undef min

#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace wrapper::directx12 {

	using Microsoft::WRL::ComPtr;
	
	size_t size_of(const DXGI_FORMAT& format);

	size_t align_to(size_t value, size_t alignment);

	D3D12_CPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle, size_t value);

	D3D12_GPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, size_t value);
	
}
