#pragma once

#include "resource.hpp"
#include "buffer.hpp"

namespace wrapper::directx12 {

	struct clear_value {
		float red = 1.f, green = 1.f, blue = 1.f, alpha = 1.f;
		float depth = 1.f;
		uint8 stencil = 0;
		
		clear_value() = default;

		clear_value(float red, float green, float blue, float alpha);

		clear_value(float depth, uint8 stencil);
	};
	
	class texture2d final : public resource {
	public:
		texture2d() = default;

		explicit texture2d(const ComPtr<ID3D12Resource>& source);

		~texture2d() = default;

		void copy_from(const graphics_command_list& command_list, const buffer& buffer) const;

		void copy_from(const graphics_command_list& command_list, const texture2d& texture) const;
		
		DXGI_FORMAT format() const;

		size_t alignment() const;
		
		size_t width() const;

		size_t height() const;
		
		static texture2d create(const device& device, const resource_info& info, 
			const DXGI_FORMAT& format, size_t width, size_t height, const clear_value& clear = clear_value());

		static texture2d create(const device& device, const resource_info& info,
			const DXGI_FORMAT& format, size_t width, size_t height, size_t samples,
			const clear_value& clear = clear_value());
	};
	
}
