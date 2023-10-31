#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <vector>
#include <string>
#include <array>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#undef max
#undef min

namespace wrapper::directx12
{
	using Microsoft::WRL::ComPtr;

	using uint64 = unsigned long long;
	using uint32 = unsigned;
	using uint8 = unsigned char;
	using int32 = int;
	using real = float;

	template <typename Key, typename Value>
	using mapping = std::unordered_map<Key, Value>;
	
	uint32 size_of(const DXGI_FORMAT& format);

	uint64 align_to(uint64 value, uint64 alignment);

	D3D12_CPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle, size_t value);

	D3D12_GPU_DESCRIPTOR_HANDLE offset_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, size_t value);

	std::string copy_data_to_string(void* data, size_t size_in_bytes);

	std::string wide_string_to_multi_bytes_string(const std::wstring& wide_string);

	std::wstring multi_bytes_string_to_wide_string(const std::string& string);
}
