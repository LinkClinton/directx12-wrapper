#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class command_allocator final : public wrapper_t<ID3D12CommandAllocator> {
	public:
		command_allocator() = default;

		explicit command_allocator(const ComPtr<ID3D12CommandAllocator>& source);

		~command_allocator() = default;

		static command_allocator create(const device& device);
	};
	
}
