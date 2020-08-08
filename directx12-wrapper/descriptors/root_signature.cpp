#include "root_signature.hpp"

const std::vector<D3D12_STATIC_SAMPLER_DESC>& wrapper::directx12::root_signature_info::samplers() const
{
	return mStaticSamplers;
}

const std::vector<D3D12_ROOT_PARAMETER>& wrapper::directx12::root_signature_info::parameters() const
{
	return mRootParameters;
}

wrapper::directx12::root_signature::root_signature(const ComPtr<ID3D12RootSignature>& source) : wrapper_t<ID3D12RootSignature>(source)
{
}

wrapper::directx12::root_signature wrapper::directx12::root_signature::create(const device& device, const root_signature_info& info, bool local)
{
	D3D12_ROOT_SIGNATURE_DESC desc;

	desc.Flags = local ? D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE : D3D12_ROOT_SIGNATURE_FLAG_NONE;
	desc.NumStaticSamplers = static_cast<UINT>(info.samplers().size());
	desc.NumParameters = static_cast<UINT>(info.parameters().size());
	desc.pStaticSamplers = info.samplers().data();
	desc.pParameters = info.parameters().data();

	ComPtr<ID3DBlob> signature_blob = nullptr;
	ComPtr<ID3DBlob> error_blob = nullptr;

	ComPtr<ID3D12RootSignature> signature;
	
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, signature_blob.GetAddressOf(), error_blob.GetAddressOf());

	device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(),
		IID_PPV_ARGS(signature.GetAddressOf()));

	return root_signature(signature);
}