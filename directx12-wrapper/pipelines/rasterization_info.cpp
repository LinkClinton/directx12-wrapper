#include "rasterization_info.hpp"

wrapper::directx12::rasterization_info::rasterization_info()
{
	mDesc.FillMode = D3D12_FILL_MODE_SOLID;
	mDesc.CullMode = D3D12_CULL_MODE_BACK;
	mDesc.FrontCounterClockwise = false;
	mDesc.DepthBias = 0;
	mDesc.DepthBiasClamp = 0.0f;
	mDesc.SlopeScaledDepthBias = 0.0f;
	mDesc.DepthClipEnable = true;
	mDesc.MultisampleEnable = false;
	mDesc.AntialiasedLineEnable = false;
	mDesc.ForcedSampleCount = 0;
	mDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

D3D12_RASTERIZER_DESC wrapper::directx12::rasterization_info::desc() const
{
	return mDesc;
}
