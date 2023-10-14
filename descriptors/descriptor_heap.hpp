#pragma once

#include "../devices/device.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace wrapper::directx12
{

	struct cpu_descriptor_handle : D3D12_CPU_DESCRIPTOR_HANDLE
	{
		cpu_descriptor_handle() = default;

		cpu_descriptor_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
		
		cpu_descriptor_handle& operator+(const cpu_descriptor_handle& rhs);
		cpu_descriptor_handle& operator-(const cpu_descriptor_handle& rhs);
		cpu_descriptor_handle& operator+(size_t offset);
		cpu_descriptor_handle& operator-(size_t offset);
	};
	
	struct gpu_descriptor_handle : D3D12_GPU_DESCRIPTOR_HANDLE
	{
		gpu_descriptor_handle() = default;

		gpu_descriptor_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle);

		gpu_descriptor_handle& operator+(const gpu_descriptor_handle& rhs);
		gpu_descriptor_handle& operator-(const gpu_descriptor_handle& rhs);
		gpu_descriptor_handle& operator+(size_t offset);
		gpu_descriptor_handle& operator-(size_t offset);
	};
	
	class descriptor_table final
	{
	public:
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
	
	class descriptor_heap final : public wrapper_t<ID3D12DescriptorHeap>
	{
	public:
		descriptor_heap() = default;

		explicit descriptor_heap(const ComPtr<ID3D12DescriptorHeap>& source, const ComPtr<ID3D12Device5>& device);
		
		D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle(size_t index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle(size_t index = 0) const;
		
		static descriptor_heap create(const device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, size_t count);

		static descriptor_heap create(const device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const D3D12_DESCRIPTOR_HEAP_FLAGS& flags, size_t count);
		
		static descriptor_heap create(const device& device, const descriptor_table& table);
	private:
		size_t mOffset = 0;
		size_t mCount = 0;
	};
	
}
