#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class resource final {
	public:
		resource() = default;

		explicit resource(const ComPtr<ID3D12Resource>& source);

		~resource() = default;

		ID3D12Resource* const* get_address_off() const;
		ID3D12Resource* operator->() const;
		ID3D12Resource* get() const;
	private:
		ComPtr<ID3D12Resource> mResource;

		D3D12_RESOURCE_DESC mDesc;
	};
	
}
