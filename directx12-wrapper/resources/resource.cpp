#include "resource.hpp"

wrapper::directx12::resource::resource(const ComPtr<ID3D12Resource>& source) : mResource(source)
{
	mDesc = mResource->GetDesc();
}

ID3D12Resource* const* wrapper::directx12::resource::get_address_off() const
{
	return mResource.GetAddressOf();
}

ID3D12Resource* wrapper::directx12::resource::operator->() const
{
	return mResource.Get();
}

ID3D12Resource* wrapper::directx12::resource::get() const
{
	return mResource.Get();
}
