#pragma once

#include "../application-sample/application.hpp"

#include "../../directx12-wrapper/pipelines/pipeline_state.hpp"
#include "../../directx12-wrapper/swap_chain.hpp"

namespace wrapper::samples {
	
	class triangle_application final : public application {
	public:
		explicit triangle_application(const std::string& name, int width, int height);

		~triangle_application() = default;
	protected:
		void update(float delta) override;

		void render(float delta) override;
	private:
		directx12::command_allocator mCommandAllocator;
		directx12::command_queue mCommandQueue;
		
		directx12::graphics_command_list mCommandList;

		directx12::input_assembly_info mInputAssemblyInfo;
		directx12::rasterization_info mRasterizationInfo;
		directx12::depth_stencil_info mDepthStencilInfo;
		directx12::blend_info mBlendInfo;
		
		directx12::root_signature_info mRootSignatureInfo;
		directx12::root_signature mRootSignature;

		directx12::shader_code mVertShader;
		directx12::shader_code mFragShader;
		
		directx12::graphics_pipeline_info mGraphicsPipelineInfo;
		directx12::pipeline_state mGraphicsPipeline;

		directx12::resource mVertexBuffer;
		
		directx12::swap_chain mSwapChain;
		directx12::device mDevice;
		directx12::fence mFence;

		directx12::descriptor_heap mRenderTargetViewHeap;
	};
	
}
