#include "adapter.hpp"

wrapper::directx12::adapter::adapter(const ComPtr<IDXGIAdapter1>& source) : wrapper_t<IDXGIAdapter1>(source)
{
	mWrapperInstance->GetDesc1(&mDesc);
}

std::vector<wrapper::directx12::adapter> wrapper::directx12::adapter::all()
{
	ComPtr<IDXGIFactory4> factory;
	ComPtr<IDXGIAdapter1> adapter;

	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));

	UINT index = 0;

	std::vector<directx12::adapter> adapters;
	
	while (factory->EnumAdapters1(index++, &adapter) != DXGI_ERROR_NOT_FOUND)
		adapters.push_back(directx12::adapter(adapter));

	return adapters;
}
