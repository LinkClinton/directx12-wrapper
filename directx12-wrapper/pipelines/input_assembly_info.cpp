#include "input_assembly_info.hpp"

void wrapper::directx12::input_assembly_info::add_input_element(const std::string& name, const DXGI_FORMAT& format, size_t slot)
{
	// cache the name to keep the life of SemanticName in mElements
	mNames.push_back(name);

	D3D12_INPUT_ELEMENT_DESC desc;

	desc.SemanticName = "";
	desc.SemanticIndex = 0;
	desc.Format = format;
	desc.InputSlot = static_cast<UINT>(slot);
	desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	desc.InstanceDataStepRate = 0;
	desc.AlignedByteOffset = static_cast<UINT>(mElements.empty() ? 0 : (mElements.back().AlignedByteOffset + size_of(mElements.back().Format)));

	mElements.push_back(desc);

	// when mNames expand the memory, the pointer of SemanticName will be invalid
	// so we need update them 
	for (size_t index = 0; index < mNames.size(); index++)
		mElements[index].SemanticName = mNames[index].c_str();
}

D3D12_INPUT_LAYOUT_DESC wrapper::directx12::input_assembly_info::desc() const
{
	return {
		mElements.data(),
		static_cast<UINT>(mElements.size())
	};
}
