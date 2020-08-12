#pragma once

#include "resource.hpp"
#include "buffer.hpp"

namespace wrapper::directx12 {

	class texture2d final : public resource {
	public:
		texture2d() = default;

		explicit texture2d(const ComPtr<ID3D12Resource>& source);

		~texture2d() = default;

		void copy_from(const graphics_command_list& command_list, const buffer& buffer) const;
		
		DXGI_FORMAT format() const;

		size_t alignment() const;
		
		size_t width() const;

		size_t height() const;
		
		static texture2d create(const device& device, const resource_info& info, 
			const DXGI_FORMAT& format, size_t width, size_t height);
	};
	
}
