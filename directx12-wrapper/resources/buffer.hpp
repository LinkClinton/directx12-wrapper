#pragma once

#include "resource.hpp"

namespace wrapper::directx12 {

	class buffer final : public resource {
	public:
		buffer() = default;

		explicit buffer(const ComPtr<ID3D12Resource>& source);

		~buffer() = default;

		void copy_from_cpu(void* data, size_t size) const;

		auto begin_mapping() const -> void*;

		void end_mapping() const;

		size_t size_in_bytes() const;

		static buffer create(const device& device, const resource_info& info, size_t size);
	};
	
}
