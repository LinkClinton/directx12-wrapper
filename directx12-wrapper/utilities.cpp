#include "utilities.hpp"

size_t wrapper::directx12::size_of(const DXGI_FORMAT& format)
{
	// todo : support more DXGI_FORMAT
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
