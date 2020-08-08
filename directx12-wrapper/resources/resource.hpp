#pragma once

#include "../commands/command_list.hpp"
#include "../device.hpp"

namespace wrapper::directx12 {

	class resource final : public wrapper_t<ID3D12Resource> {
	public:
		resource() = default;

		explicit resource(const ComPtr<ID3D12Resource>& source);

		~resource() = default;

		void copy_data_from_cpu(void* data, size_t size) const;
		
		auto barrier(
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const
			-> D3D12_RESOURCE_BARRIER ;
		
		void barrier(
			const graphics_command_list& command_list,
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const;

		static resource create(
			const device& device,
			const D3D12_RESOURCE_STATES& state, 
			const D3D12_RESOURCE_FLAGS& flags, 
			const D3D12_HEAP_TYPE& type, size_t size);
	private:
		D3D12_RESOURCE_DESC mDesc;

		D3D12_HEAP_PROPERTIES mHeapProperties;
		D3D12_HEAP_FLAGS mHeapFlags;
	};
	
}
