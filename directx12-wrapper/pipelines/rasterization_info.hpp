#pragma once

#include "../devices/device.hpp"

namespace wrapper::directx12 {

	class rasterization_info final {
	public:
		rasterization_info();

		~rasterization_info() = default;

		rasterization_info& set_fill_mode(const D3D12_FILL_MODE& mode);

		rasterization_info& set_cull_mode(const D3D12_CULL_MODE& mode);

		rasterization_info& set_multi_sample_enable(bool enable);
		
		D3D12_RASTERIZER_DESC desc() const;
	private:
		D3D12_RASTERIZER_DESC mDesc = {};
	};

}
