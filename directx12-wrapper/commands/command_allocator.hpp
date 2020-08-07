#pragma once

#include "../device.hpp"

namespace wrapper::directx12 {

	class command_allocator final {
	public:
		command_allocator() = default;

		explicit command_allocator(const ComPtr<ID3D12CommandAllocator>& source);

		~command_allocator() = default;

		ID3D12CommandAllocator* const* get_address_off() const;
		ID3D12CommandAllocator* operator->() const;
		ID3D12CommandAllocator* get() const;

		static command_allocator create(const device& device);
	private:
		ComPtr<ID3D12CommandAllocator> mAllocator;
	};
	
}
