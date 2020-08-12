#pragma once

#include "../commands/command_list.hpp"
#include "../device.hpp"

namespace wrapper::directx12 {

	class resource_info final {
	public:
		resource_info() = default;

		explicit resource_info(
			const D3D12_RESOURCE_STATES& state,
			const D3D12_RESOURCE_FLAGS& flags,
			const D3D12_HEAP_TYPE& type);
		
		~resource_info() = default;

		resource_info& set_resource_state(const D3D12_RESOURCE_STATES& state);

		resource_info& set_resource_flags(const D3D12_RESOURCE_FLAGS& flags);

		resource_info& set_heap_type(const D3D12_HEAP_TYPE& type);

		D3D12_RESOURCE_STATES state() const;

		D3D12_RESOURCE_FLAGS flags() const;

		D3D12_HEAP_TYPE type() const;

		static resource_info upload(
			const D3D12_RESOURCE_STATES& state = D3D12_RESOURCE_STATE_GENERIC_READ,
			const D3D12_RESOURCE_FLAGS& flags = D3D12_RESOURCE_FLAG_NONE);

		static resource_info common(
			const D3D12_RESOURCE_STATES& state = D3D12_RESOURCE_STATE_GENERIC_READ,
			const D3D12_RESOURCE_FLAGS& flags = D3D12_RESOURCE_FLAG_NONE);
	private:
		D3D12_RESOURCE_STATES mState;
		D3D12_RESOURCE_FLAGS mFlags;
		D3D12_HEAP_TYPE mHeapType;
	};

	class resource : public wrapper_t<ID3D12Resource> {
	public:
		resource() = default;

		explicit resource(const ComPtr<ID3D12Resource>& source);
		
		~resource() = default;

		auto barrier(
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const
			->D3D12_RESOURCE_BARRIER;

		void barrier(
			const graphics_command_list& command_list,
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const;
	protected:
		D3D12_RESOURCE_DESC mDesc;

		D3D12_HEAP_PROPERTIES mHeapProperties;
		D3D12_HEAP_FLAGS mHeapFlags;
	};
}
