#pragma once

#include "../devices/device.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace wrapper::directx12 {

	class descriptor_table final {
	public:
		descriptor_table() = default;

		~descriptor_table() = default;
		
		descriptor_table& add_srv_range(const std::vector<std::string>& name, size_t base, size_t space);

		descriptor_table& add_uav_range(const std::vector<std::string>& name, size_t base, size_t space);

		descriptor_table& add_cbv_range(const std::vector<std::string>& name, size_t base, size_t space);

		descriptor_table& add_range(const std::vector<std::string>& name, const D3D12_DESCRIPTOR_RANGE_TYPE& type, size_t base, size_t space);

		D3D12_ROOT_PARAMETER parameter() const;

		size_t index(const std::string& name) const;

		size_t count() const;
	private:
		std::unordered_map<std::string, size_t> mDescriptorsIndex;

		std::vector<D3D12_DESCRIPTOR_RANGE> mRanges;
	};
	
	class descriptor_heap final : public wrapper_t<ID3D12DescriptorHeap> {
	public:
		descriptor_heap() = default;

		explicit descriptor_heap(const ComPtr<ID3D12DescriptorHeap>& source, const ComPtr<ID3D12Device5>& device);
		
		~descriptor_heap() = default;

		D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle(size_t index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle(size_t index = 0) const;
		
		static descriptor_heap create(const device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, size_t count);

		static descriptor_heap create(const device& device, const descriptor_table& table);
	private:
		size_t mOffset = 0;
		size_t mCount = 0;
	};
	
}
