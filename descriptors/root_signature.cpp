#include "root_signature.hpp"

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_descriptor(
	const std::string& name, const D3D12_ROOT_PARAMETER_TYPE& type, uint32 base, uint32 space)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	parameter.ParameterType = type;
	parameter.Descriptor.ShaderRegister = base;
	parameter.Descriptor.RegisterSpace = space;

	return add_root_parameter(name, parameter);
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_descriptor_table(
	const std::string& name, const descriptor_table& table)
{
	return add_root_parameter(name, table.parameter());
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_constants(
	const std::string& name, uint32 base, uint32 space, uint32 count)
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	parameter.Constants.Num32BitValues = count;
	parameter.Constants.ShaderRegister = base;
	parameter.Constants.RegisterSpace = space;

	return add_root_parameter(name, parameter);
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_unordered_access_view(
	const std::string& name, uint32 base, uint32 space)
{
	return add_descriptor(name, D3D12_ROOT_PARAMETER_TYPE_UAV, base, space);
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_shader_resource_view(
	const std::string& name, uint32 base, uint32 space)
{
	return add_descriptor(name, D3D12_ROOT_PARAMETER_TYPE_SRV, base, space);
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_constant_buffer_view(
	const std::string& name, uint32 base, uint32 space)
{
	return add_descriptor(name, D3D12_ROOT_PARAMETER_TYPE_CBV, base, space);
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_static_sampler(
	const std::string& name, uint32 base, uint32 space)
{
	D3D12_STATIC_SAMPLER_DESC desc;

	desc.RegisterSpace = space;
	desc.ShaderRegister = base;
	desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.MipLODBias = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D12_FLOAT32_MAX;
	desc.MaxAnisotropy = 0;
	desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	desc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	mStaticSamplers.push_back(desc);

	return *this;
}

wrapper::directx12::uint32 wrapper::directx12::root_signature_info::index(const std::string& name) const
{
	return mDescriptorIndex.at(name);
}

const std::vector<D3D12_STATIC_SAMPLER_DESC>& wrapper::directx12::root_signature_info::samplers() const
{
	return mStaticSamplers;
}

const std::vector<D3D12_ROOT_PARAMETER>& wrapper::directx12::root_signature_info::parameters() const
{
	return mRootParameters;
}

size_t wrapper::directx12::root_signature_info::base(const std::string& name) const
{
	return mDescriptorBase[mDescriptorIndex.at(name)];
}

size_t wrapper::directx12::root_signature_info::size(const std::string& name) const
{
	return mDescriptorSize[mDescriptorIndex.at(name)];
}

size_t wrapper::directx12::root_signature_info::size() const
{
	return mSize;
}

wrapper::directx12::root_signature_info& wrapper::directx12::root_signature_info::add_root_parameter(
	const std::string& name, const D3D12_ROOT_PARAMETER& parameter)
{
	mRootParameters.push_back(parameter);

	mDescriptorIndex.insert({ name, static_cast<uint32>(mDescriptorIndex.size()) });

	const auto size = parameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS ?
		align_to(static_cast<size_t>(parameter.Constants.Num32BitValues) * 4, sizeof(D3D12_GPU_DESCRIPTOR_HANDLE)) :
		sizeof(D3D12_GPU_DESCRIPTOR_HANDLE);

	mDescriptorBase.push_back(mSize);
	mDescriptorSize.push_back(size);

	mSize = mSize + size;

	return *this;
}

wrapper::directx12::root_signature::root_signature(const ComPtr<ID3D12RootSignature>& source) : wrapper_t<ID3D12RootSignature>(source)
{
}

wrapper::directx12::root_signature wrapper::directx12::root_signature::create(const device& device, const root_signature_info& info, bool local)
{
	D3D12_ROOT_SIGNATURE_DESC desc;

	// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_root_signature_flags
	// This flag(D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE) cannot be combined with any other root signature flags
	desc.Flags = local ? D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE : D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.NumStaticSamplers = static_cast<UINT>(info.samplers().size());
	desc.NumParameters = static_cast<UINT>(info.parameters().size());
	desc.pStaticSamplers = info.samplers().data();
	desc.pParameters = info.parameters().data();

	ComPtr<ID3DBlob> signature_blob = nullptr;
	ComPtr<ID3DBlob> error_blob = nullptr;

	ComPtr<ID3D12RootSignature> signature;
	
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, signature_blob.GetAddressOf(), error_blob.GetAddressOf());

	// output error message if we have
	if (error_blob != nullptr) {
		auto error = copy_data_to_string(error_blob->GetBufferPointer(), error_blob->GetBufferSize());

		if (!error.empty() && error.back() == '\n') error.pop_back();
		if (!error.empty()) directx12::error(error);
	}
	
	device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(),
		IID_PPV_ARGS(signature.GetAddressOf()));

	return root_signature(signature);
}