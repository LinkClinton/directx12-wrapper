#pragma once

#include "resource.hpp"

namespace wrapper::directx12
{

	class buffer final : public resource
	{
	public:
		buffer() = default;

		explicit buffer(const ComPtr<ID3D12Resource>& source);

		void copy_from_cpu(const void* data, size_t size) const;

		void copy_from(const graphics_command_list& command_list, const buffer& buffer) const;

		void copy_from(const graphics_command_list& command_list, const texture2d& texture) const;
		
		auto begin_mapping() const -> void*;

		void end_mapping() const;

		size_t size_in_bytes() const;

		static buffer create(const device& device, const resource_info& info, size_t size);
	};
	
}
