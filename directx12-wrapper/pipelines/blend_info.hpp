#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class blend_info final {
	public:
		blend_info();

		~blend_info() = default;

		blend_info& set_render_target(const D3D12_RENDER_TARGET_BLEND_DESC& desc, size_t index);
		
		D3D12_BLEND_DESC desc() const;
	private:
		D3D12_BLEND_DESC mDesc;
	};
	
}
