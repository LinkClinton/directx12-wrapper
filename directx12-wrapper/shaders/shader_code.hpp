#pragma once

#include "../device.hpp"

#include <vector>
#include <string>

#define __ENABLE_DIRECTX_COMPILER__

namespace wrapper::directx12 {

	class shader_code final {
	public:
		shader_code() = default;

		explicit shader_code(const std::vector<byte>& code);
		
		~shader_code() = default;

		const byte* data() const;

		size_t size() const;

		static shader_code create(const std::vector<byte>& code);

#ifdef __ENABLE_DIRECTX_COMPILER__
		static shader_code create(const std::wstring& filename);
#endif
	private:
		std::vector<byte> mCode;
	};
	
}
