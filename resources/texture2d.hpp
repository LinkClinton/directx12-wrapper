#pragma once

#include "resource.hpp"
#include "buffer.hpp"

namespace wrapper::directx12
{

	struct clear_value
	{
		float red = 1.f, green = 1.f, blue = 1.f, alpha = 1.f;
		float depth = 1.f;
		uint8 stencil = 0;
		
		clear_value() = default;

		clear_value(float red, float green, float blue, float alpha);

		clear_value(float depth, uint8 stencil);
	};
	
	class texture2d final : public resource
	{
	public:
		texture2d() = default;

		explicit texture2d(const ComPtr<ID3D12Resource>& source);

		void copy_from(const graphics_command_list& command_list, const buffer& buffer) const;

		void copy_from(const graphics_command_list& command_list, const texture2d& texture) const;
		
		DXGI_FORMAT format() const;

		uint32 alignment() const;
		
		uint32 size_x() const;

		uint32 size_y() const;
		
		static texture2d create(const device& device, const resource_info& info, 
			const DXGI_FORMAT& format, uint32 size_x, uint32 size_y, const clear_value& clear = clear_value());

		static texture2d create(const device& device, const resource_info& info,
			const DXGI_FORMAT& format, uint32 size_x, uint32 size_y, uint32 samples,
			const clear_value& clear = clear_value());
	};
	
}
