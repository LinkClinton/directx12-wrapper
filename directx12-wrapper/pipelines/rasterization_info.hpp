#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class rasterization_info final {
	public:
		rasterization_info();

		~rasterization_info() = default;

		D3D12_RASTERIZER_DESC desc() const;
	private:
		D3D12_RASTERIZER_DESC mDesc = {};
	};

}
