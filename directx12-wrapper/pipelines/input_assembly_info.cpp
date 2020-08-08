#include "input_assembly_info.hpp"

wrapper::directx12::input_assembly_info& wrapper::directx12::input_assembly_info::add_input_element(const std::string& name, const DXGI_FORMAT& format, size_t slot)
{
	// cache the name to keep the life of SemanticName in mElements
	mNames.push_back(name);

	D3D12_INPUT_ELEMENT_DESC desc;

	desc.SemanticName = mNames.back().c_str();
	desc.SemanticIndex = 0;
	desc.Format = format;
	desc.InputSlot = static_cast<UINT>(slot);
	desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	desc.InstanceDataStepRate = 0;
	desc.AlignedByteOffset = static_cast<UINT>(mElements.empty() ? 0 : (mElements.back().AlignedByteOffset + size_of(mElements.back().Format)));

	mElements.push_back(desc);

	return *this;
}

D3D12_INPUT_LAYOUT_DESC wrapper::directx12::input_assembly_info::desc() const
{
	return {
		mElements.data(),
		static_cast<UINT>(mElements.size())
	};
}
