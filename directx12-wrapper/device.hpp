#pragma once

#include "interfaces/wrapper.hpp"

namespace wrapper::directx12 {

	class device final : public wrapper_t<ID3D12Device5> {
	public:
		device() = default;

		explicit device(const ComPtr<ID3D12Device5>& source);
		
		~device() = default;

		static device create(const D3D_FEATURE_LEVEL& level);
	};
	
}
