#pragma once

#include "../devices/device.hpp"

#include <vector>
#include <string>

namespace wrapper::directx12 {

	class shader_code final {
	public:
		shader_code() = default;

		explicit shader_code(const std::vector<byte>& code);
		
		~shader_code() = default;

		const byte* data() const;

		size_t size() const;

		static shader_code create(const std::vector<byte>& code);

		static shader_code create_from_source(const std::string& source, const std::string& entry, const std::string& version);
	private:
		std::vector<byte> mCode;
	};

}
