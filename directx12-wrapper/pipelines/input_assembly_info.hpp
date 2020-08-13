#pragma once

#include "../devices/device.hpp"

#include <vector>
#include <string>

namespace wrapper::directx12 {

	class input_assembly_info final {
	public:
		input_assembly_info() = default;

		~input_assembly_info() = default;

		input_assembly_info& add_input_element(const std::string& name, const DXGI_FORMAT& format, size_t slot = 0);

		D3D12_INPUT_LAYOUT_DESC desc() const;
	private:
		std::vector<D3D12_INPUT_ELEMENT_DESC> mElements;
		std::vector<std::string> mNames;
	};
	
}
