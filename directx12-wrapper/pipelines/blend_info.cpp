#include "blend_info.hpp"

wrapper::directx12::blend_info::blend_info()
{
	mDesc.AlphaToCoverageEnable = false;
	mDesc.IndependentBlendEnable = false;

	mDesc.RenderTarget[0].BlendEnable = false;
	mDesc.RenderTarget[0].LogicOpEnable = false;
	mDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	mDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	mDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	mDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	mDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	mDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	mDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	mDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

D3D12_BLEND_DESC wrapper::directx12::blend_info::desc() const
{
	return mDesc;
}
