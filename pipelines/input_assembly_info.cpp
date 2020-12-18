#include "input_assembly_info.hpp"

wrapper::directx12::input_assembly_info::input_assembly_info(const input_assembly_info& info) :
	mSlotsOffset(info.mSlotsOffset), mElements(info.mElements), mNames(info.mNames)
{
	for (size_t index = 0; index < mNames.size(); index++)
		mElements[index].SemanticName = mNames[index].c_str();
}

wrapper::directx12::input_assembly_info& wrapper::directx12::input_assembly_info::add_input_element(
	const std::string& name, const DXGI_FORMAT& format, uint32 slot)
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
	desc.AlignedByteOffset = mSlotsOffset[slot];

	mSlotsOffset[slot] += size_of(format);
	
	mElements.push_back(desc);

	// when mNames expand the memory, the pointer of SemanticName will be invalid
	// so we need update them 
	for (size_t index = 0; index < mNames.size(); index++)
		mElements[index].SemanticName = mNames[index].c_str();

	return *this;
}

D3D12_INPUT_LAYOUT_DESC wrapper::directx12::input_assembly_info::desc() const
{
	return {
		mElements.data(),
		static_cast<UINT>(mElements.size())
	};
}

wrapper::directx12::input_assembly_info& wrapper::directx12::input_assembly_info::operator=(
	const input_assembly_info& info)
{
	mSlotsOffset = info.mSlotsOffset;
	mElements = info.mElements;
	mNames = info.mNames;

	for (size_t index = 0; index < mNames.size(); index++)
		mElements[index].SemanticName = mNames[index].c_str();

	return *this;
}
