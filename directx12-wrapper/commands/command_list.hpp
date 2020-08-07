#pragma once

#include "command_allocator.hpp"

namespace wrapper::directx12 {

	class graphics_command_list final {
	public:
		graphics_command_list() = default;

		explicit graphics_command_list(const ComPtr<ID3D12GraphicsCommandList4>& source);

		~graphics_command_list() = default;

		ID3D12GraphicsCommandList4* const* get_address_off() const;
		ID3D12GraphicsCommandList4* operator->() const;
		ID3D12GraphicsCommandList4* get() const;

		static graphics_command_list create(const device& device, const command_allocator& allocator);
	private:
		ComPtr<ID3D12GraphicsCommandList4> mCommandList;
	};
	
}
