#include "pipeline_state.hpp"

wrapper::directx12::graphics_pipeline_info::graphics_pipeline_info()
{
	mDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	mDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	mDesc.NodeMask = 0;
	mDesc.SampleDesc.Count = 1;
	mDesc.SampleDesc.Quality = 0;
	mDesc.SampleMask = UINT_MAX;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_input_assembly(const input_assembly_info& input_assembly)
{
	mDesc.InputLayout = input_assembly.desc();

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_depth_stencil(const depth_stencil_info& depth_stencil)
{
	mDesc.DepthStencilState = depth_stencil.desc();

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_rasterization(const rasterization_info& rasterization)
{
	mDesc.RasterizerState = rasterization.desc();

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_multi_sample(size_t samples, size_t quality)
{
	mDesc.SampleDesc.Count = static_cast<UINT>(samples);
	mDesc.SampleDesc.Quality = static_cast<UINT>(quality);

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_blend(const blend_info& blend)
{
	mDesc.BlendState = blend.desc();

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_vert_shader(const shader_code& shader)
{
	mDesc.VS.pShaderBytecode = shader.data();
	mDesc.VS.BytecodeLength = static_cast<SIZE_T>(shader.size());

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_frag_shader(const shader_code& shader)
{
	mDesc.PS.pShaderBytecode = shader.data();
	mDesc.PS.BytecodeLength = static_cast<SIZE_T>(shader.size());

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_root_signature(const root_signature& signature)
{
	mDesc.pRootSignature = signature.get();

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_primitive_type(
	const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type)
{
	mDesc.PrimitiveTopologyType = type;

	return *this;
}

wrapper::directx12::graphics_pipeline_info& wrapper::directx12::graphics_pipeline_info::set_format(
	const std::vector<DXGI_FORMAT>& render_targets, const DXGI_FORMAT& depth_stencil)
{
	assert(render_targets.size() <= 8);

	mDesc.NumRenderTargets = static_cast<UINT>(render_targets.size());

	for (size_t index = 0; index < render_targets.size(); index++)
		mDesc.RTVFormats[index] = render_targets.at(index);

	mDesc.DSVFormat = depth_stencil;
	
	return *this;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC wrapper::directx12::graphics_pipeline_info::desc() const
{
	return mDesc;
}

wrapper::directx12::compute_pipeline_info::compute_pipeline_info()
{
	mDesc.NodeMask = 0;
	mDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
}

wrapper::directx12::compute_pipeline_info& wrapper::directx12::compute_pipeline_info::set_root_signature(const root_signature& signature)
{
	mDesc.pRootSignature = signature.get();

	return *this;
}

wrapper::directx12::compute_pipeline_info& wrapper::directx12::compute_pipeline_info::set_compute_shader(const shader_code& shader)
{
	mDesc.CS.pShaderBytecode = shader.data();
	mDesc.CS.BytecodeLength = static_cast<SIZE_T>(shader.size());

	return *this;
}

D3D12_COMPUTE_PIPELINE_STATE_DESC wrapper::directx12::compute_pipeline_info::desc() const
{
	return mDesc;
}

wrapper::directx12::pipeline_state::pipeline_state(const ComPtr<ID3D12PipelineState>& source) :	wrapper_t<ID3D12PipelineState>(source)
{
}

wrapper::directx12::pipeline_state wrapper::directx12::pipeline_state::create(const device& device, const graphics_pipeline_info& info)
{
	const auto desc = info.desc();

	ComPtr<ID3D12PipelineState> pipeline;
	
	device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipeline.GetAddressOf()));

	return pipeline_state(pipeline);
}

wrapper::directx12::pipeline_state wrapper::directx12::pipeline_state::create(const device& device, const compute_pipeline_info& info)
{
	const auto desc = info.desc();

	ComPtr<ID3D12PipelineState> pipeline;

	device->CreateComputePipelineState(&desc, IID_PPV_ARGS(pipeline.GetAddressOf()));

	return pipeline_state(pipeline);
}
