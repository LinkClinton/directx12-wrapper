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

wrapper::directx12::rasterization_info& wrapper::directx12::rasterization_info::set_fill_mode(
	const D3D12_FILL_MODE& mode)
{
	mDesc.FillMode = mode;

	return *this;
}

wrapper::directx12::rasterization_info& wrapper::directx12::rasterization_info::set_cull_mode(
	const D3D12_CULL_MODE& mode)
{
	mDesc.CullMode = mode;

	return *this;
}

wrapper::directx12::rasterization_info& wrapper::directx12::rasterization_info::set_multi_sample_enable(bool enable)
{
	mDesc.MultisampleEnable = enable;

	return *this;
}

wrapper::directx12::rasterization_info& wrapper::directx12::rasterization_info::set_anti_aliased_line_enable(bool enable)
{
	mDesc.AntialiasedLineEnable = enable;

	return *this;
}

D3D12_RASTERIZER_DESC wrapper::directx12::rasterization_info::desc() const
{
	return mDesc;
}
