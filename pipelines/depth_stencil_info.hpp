#pragma once

#include "../devices/device.hpp"

namespace wrapper::directx12
{

	class depth_stencil_info final
	{
	public:
		depth_stencil_info();

		depth_stencil_info& set_depth_enable(bool enable);

		depth_stencil_info& set_depth_write_mask(const D3D12_DEPTH_WRITE_MASK& mask);

		depth_stencil_info& set_depth_function(const D3D12_COMPARISON_FUNC& function);

		depth_stencil_info& set_depth_all_configs(
			const D3D12_COMPARISON_FUNC& function, const D3D12_DEPTH_WRITE_MASK& mask, bool enable);
		
		D3D12_DEPTH_STENCIL_DESC desc() const;
	private:
		D3D12_DEPTH_STENCIL_DESC mDesc = {};
	};
	
}
