#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class depth_stencil_info final {
	public:
		depth_stencil_info();

		~depth_stencil_info() = default;

		D3D12_DEPTH_STENCIL_DESC desc() const;
	private:
		D3D12_DEPTH_STENCIL_DESC mDesc = {};
	};
	
}
