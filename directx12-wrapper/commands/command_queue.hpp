#pragma once

#include "command_list.hpp"

namespace wrapper::directx12 {

	class command_queue final {
	public:
		command_queue() = default;

		explicit command_queue(const ComPtr<ID3D12CommandQueue>& source);

		~command_queue() = default;

		ID3D12CommandQueue* const* get_address_off() const;
		ID3D12CommandQueue* operator->() const;
		ID3D12CommandQueue* get() const;

		static command_queue create(const device& device);
	private:
		ComPtr<ID3D12CommandQueue> mQueue;
	};
	
}
