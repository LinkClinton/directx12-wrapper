#pragma once

#include "command_list.hpp"
#include "../fence.hpp"

#include <vector>

namespace wrapper::directx12 {

	class command_queue final : public wrapper_t<ID3D12CommandQueue> {
	public:
		command_queue() = default;

		explicit command_queue(const ComPtr<ID3D12CommandQueue>& source);

		~command_queue() = default;

		void execute(const std::vector<graphics_command_list>& command_lists) const;
		
		void wait(const fence& fence) const;
		
		static command_queue create(const device& device);
	};
	
}
