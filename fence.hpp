#pragma once

#include "devices/device.hpp"

namespace wrapper::directx12 {

	class fence final : public wrapper_t<ID3D12Fence> {
	public:
		fence() = default;

		explicit fence(const ComPtr<ID3D12Fence>& source);
		
		~fence() = default;

		static fence create(const device& device, size_t value);
	};
}
