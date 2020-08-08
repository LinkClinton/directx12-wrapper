#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class rasterization_info final {
	public:
		rasterization_info();

		~rasterization_info() = default;

		rasterization_info& set_fill_mode(const D3D12_FILL_MODE& mode);

		rasterization_info& set_cull_mode(const D3D12_CULL_MODE& mode);
		
		D3D12_RASTERIZER_DESC desc() const;
	private:
		D3D12_RASTERIZER_DESC mDesc = {};
	};

}
