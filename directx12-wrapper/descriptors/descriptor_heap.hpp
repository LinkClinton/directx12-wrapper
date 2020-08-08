#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class descriptor_heap final : public wrapper_t<ID3D12DescriptorHeap> {
	public:
		descriptor_heap() = default;

		explicit descriptor_heap(const ComPtr<ID3D12DescriptorHeap>& source, const ComPtr<ID3D12Device5>& device);
		
		~descriptor_heap() = default;

		D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle(size_t index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle(size_t index = 0) const;
		
		static descriptor_heap create(const device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, size_t count);
	private:
		size_t mOffset;
		size_t mCount;
	};
	
}
