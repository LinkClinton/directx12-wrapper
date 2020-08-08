#pragma once

#include "input_assembly_info.hpp"
#include "depth_stencil_info.hpp"
#include "rasterization_info.hpp"
#include "blend_info.hpp"

#include "../descriptors/root_signature.hpp"

#include "../shaders/shader_code.hpp"
#include "../device.hpp"

namespace wrapper::directx12 {

	class graphics_pipeline_info final {
	public:
		graphics_pipeline_info();

		~graphics_pipeline_info() = default;

		graphics_pipeline_info& set_input_assembly(const input_assembly_info& input_assembly);

		graphics_pipeline_info& set_depth_stencil(const depth_stencil_info& depth_stencil);

		graphics_pipeline_info& set_rasterization(const rasterization_info& rasterization);

		graphics_pipeline_info& set_blend(const blend_info& blend);

		graphics_pipeline_info& set_vert_shader(const shader_code& shader);

		graphics_pipeline_info& set_frag_shader(const shader_code& shader);

		graphics_pipeline_info& set_render_target(const std::vector<DXGI_FORMAT>& formats);

		graphics_pipeline_info& set_depth_stencil(const DXGI_FORMAT& format);

		graphics_pipeline_info& set_root_signature(const root_signature& signature);
		
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc() const;
	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC mDesc = {};
	};
	
	class pipeline_state final : public wrapper_t<ID3D12PipelineState> {
	public:
		pipeline_state() = default;

		pipeline_state(const ComPtr<ID3D12PipelineState>& source);
		
		~pipeline_state() = default;

		static pipeline_state create(const device& device, const graphics_pipeline_info& info);
	};
	
}
