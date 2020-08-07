#pragma once

#include "utilities.hpp"

namespace wrapper::directx12 {

	class device final {
	public:
		device() = default;

		explicit device(const ComPtr<ID3D12Device5>& source);
		
		~device() = default;

		ID3D12Device5* const* get_address_off() const;
		ID3D12Device5* operator->() const;
		ID3D12Device5* get() const;

		static device create(const D3D_FEATURE_LEVEL& level);
	private:
		ComPtr<ID3D12Device5> mDevice = nullptr;
	};
	
}
