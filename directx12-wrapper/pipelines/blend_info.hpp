#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class blend_info final {
	public:
		blend_info();

		~blend_info() = default;

		D3D12_BLEND_DESC desc() const;
	private:
		D3D12_BLEND_DESC mDesc;
	};
	
}
