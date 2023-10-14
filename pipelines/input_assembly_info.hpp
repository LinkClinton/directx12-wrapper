#pragma once

#include "../devices/device.hpp"

#include <vector>
#include <string>

namespace wrapper::directx12
{

	class input_assembly_info final
	{
	public:
		input_assembly_info() = default;

		input_assembly_info(const input_assembly_info& info);

		input_assembly_info(input_assembly_info&& info) noexcept = default;
		
		input_assembly_info& add_input_element(const std::string& name, const DXGI_FORMAT& format, uint32 slot = 0);

		D3D12_INPUT_LAYOUT_DESC desc() const;

		input_assembly_info& operator=(const input_assembly_info& info);
		input_assembly_info& operator=(input_assembly_info&& info) noexcept = default;
	private:
		mapping<uint32, uint32> mSlotsOffset;
		
		std::vector<D3D12_INPUT_ELEMENT_DESC> mElements;
		std::vector<std::string> mNames;
	};
	
}
