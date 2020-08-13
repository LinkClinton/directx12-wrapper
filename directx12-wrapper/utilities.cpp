#include "utilities.hpp"

size_t wrapper::directx12::size_of(const DXGI_FORMAT& format)
{
	// todo : support more DXGI_FORMAT
	switch (format) {
	case DXGI_FORMAT_R8G8B8A8_UNORM: return 4;
	case DXGI_FORMAT_R32G32_FLOAT: return 8;
	default:
		throw "";
	}
	return 0;
}

size_t wrapper::directx12::align_to(size_t value, size_t alignment)
{
	return ((value + alignment - 1) / alignment) * alignment;
}

D3D12_CPU_DESCRIPTOR_HANDLE wrapper::directx12::offset_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle, size_t value)
{
	return { handle.ptr + value };
}

D3D12_GPU_DESCRIPTOR_HANDLE wrapper::directx12::offset_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, size_t value)
{
	return { handle.ptr + value };
}

std::string wrapper::directx12::copy_data_to_string(void* data, size_t size_in_bytes)
{
	std::string value; value.resize(size_in_bytes);

	std::memcpy(value.data(), data, size_in_bytes);

	return value;
}

std::string wrapper::directx12::wide_string_to_multi_bytes_string(const std::wstring& wide_string)
{
	const auto size = WideCharToMultiByte(
		CP_ACP, 0, wide_string.c_str(),
		-1, nullptr, 0, nullptr, nullptr);

	std::string result; result.resize(size);
	
	WideCharToMultiByte(
		CP_ACP, 0, wide_string.c_str(),
		-1, result.data(), size, nullptr, nullptr);

	return result;
}
