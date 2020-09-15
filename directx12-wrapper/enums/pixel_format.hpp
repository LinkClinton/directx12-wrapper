#pragma once

#include "../utilities.hpp"

namespace wrapper::directx12 {

	struct pixel_format {
	public:
		pixel_format() = default;

		pixel_format(const DXGI_FORMAT& format) : mFormat(format) {}

		operator DXGI_FORMAT() const { return mFormat; }

		constexpr static inline DXGI_FORMAT unknown = DXGI_FORMAT_UNKNOWN;
	private:
		DXGI_FORMAT mFormat;
	};
	
}
