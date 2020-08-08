#pragma once

#include "command_allocator.hpp"

namespace wrapper::directx12 {

	class graphics_command_list final : public wrapper_t<ID3D12GraphicsCommandList4> {
	public:
		graphics_command_list() = default;

		explicit graphics_command_list(const ComPtr<ID3D12GraphicsCommandList4>& source);

		~graphics_command_list() = default;

		static graphics_command_list create(const device& device, const command_allocator& allocator);
	};
	
}
