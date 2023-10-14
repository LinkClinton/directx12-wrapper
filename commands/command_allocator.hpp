#pragma once

#include "../devices/device.hpp"

namespace wrapper::directx12
{

	class command_allocator final : public wrapper_t<ID3D12CommandAllocator>
	{
	public:
		command_allocator() = default;

		explicit command_allocator(const ComPtr<ID3D12CommandAllocator>& source);

		static command_allocator create(const device& device);
	};
	
}
