#include "swap_chain.hpp"

wrapper::directx12::swap_chain::swap_chain(const ComPtr<IDXGISwapChain4>& source) : wrapper_t<IDXGISwapChain4>(source)
{
	mWrapperInstance->GetDesc(&mDesc);

	for (UINT index = 0; index < mDesc.BufferCount; index++) {
		ComPtr<ID3D12Resource> buffer;
		
		mWrapperInstance->GetBuffer(index, IID_PPV_ARGS(buffer.GetAddressOf()));

		mTextures.push_back(texture2d(buffer));
	}
}

void wrapper::directx12::swap_chain::present(bool sync) const
{
	mWrapperInstance->Present(sync ? 1 : 0, sync ? 0 : DXGI_PRESENT_ALLOW_TEARING);
}

DXGI_FORMAT wrapper::directx12::swap_chain::format() const
{
	return mDesc.BufferDesc.Format;
}

const std::vector<wrapper::directx12::texture2d>& wrapper::directx12::swap_chain::buffers() const
{
	return mTextures;
}

wrapper::directx12::swap_chain wrapper::directx12::swap_chain::create(const command_queue& queue, int size_x, int size_y, HWND handle)
{
	DXGI_SWAP_CHAIN_DESC desc;

	desc.BufferCount = static_cast<UINT>(2);
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Width = static_cast<UINT>(size_x);
	desc.BufferDesc.Height = static_cast<UINT>(size_y);
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	desc.OutputWindow = handle;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Windowed = true;

	ComPtr<IDXGIFactory4> factory;
	ComPtr<IDXGISwapChain> swap_chain0;
	ComPtr<IDXGISwapChain4> swap_chain4;

	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));

	factory->CreateSwapChain(queue.get(), &desc, swap_chain0.GetAddressOf());

	factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);
	
	swap_chain0->QueryInterface(IID_PPV_ARGS(swap_chain4.GetAddressOf()));

	return swap_chain(swap_chain4);
}
