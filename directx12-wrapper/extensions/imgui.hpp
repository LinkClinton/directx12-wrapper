#pragma once

#define __DIRECTX_WRAPPER_EXTENSION_IMGUI__

#ifdef __DIRECTX_WRAPPER_EXTENSION_IMGUI__

#include "../descriptors/root_signature.hpp"
#include "../pipelines/pipeline_state.hpp"
#include "../resources/resource.hpp"

#include <imgui.h>

namespace wrapper::directx12::extensions {

	class imgui_frame_resources {
	public:
		explicit imgui_frame_resources(const device& device);

		~imgui_frame_resources() = default;

		void update(ImDrawData* draw_data);

		resource vtx_buffer() const;

		resource idx_buffer() const;
	private:
		static inline const size_t VtxBufferIncrease = 500;
		static inline const size_t IdxBufferIncrease = 5000;

		device mDevice;

		resource mVtxBuffer;
		resource mIdxBuffer;

		size_t mCurrentVtxBufferCount = 0;
		size_t mCurrentIdxBufferCount = 0;
	};
	
	class imgui_context final {
	public:
		imgui_context() = delete;

		~imgui_context() = delete;

		static void initialize(
			const device& device, const descriptor_heap& heap,
			const D3D12_CPU_DESCRIPTOR_HANDLE& font_cpu_handle,
			const D3D12_GPU_DESCRIPTOR_HANDLE& font_gpu_handle,
			const DXGI_FORMAT& render_target_view_format,
			size_t num_frames_in_flight);

		static void new_frame();

		static void render(ImDrawData* draw_data, const graphics_command_list& command_list);
	private:
		static void create_pipeline_state();

		static void create_font_texture();
	private:
		static inline device Device;

		static inline root_signature_info RootSignatureInfo;
		static inline root_signature RootSignature;
		
		static inline pipeline_state GraphicsPipeline;

		static inline descriptor_table DescriptorTable;
		static inline descriptor_heap DescriptorHeap;

		static inline shader_code VertShader;
		static inline shader_code FragShader;
		
		static inline resource FontTexture;
		
		static inline std::vector<imgui_frame_resources> FrameResources;

		static inline D3D12_CPU_DESCRIPTOR_HANDLE FontCpuHandle;
		static inline D3D12_GPU_DESCRIPTOR_HANDLE FontGpuHandle;
		static inline DXGI_FORMAT RenderTargetViewFormat;
		
		static inline size_t CurrentFrameIndex = 0;
	};

	inline imgui_frame_resources::imgui_frame_resources(const device& device) : mDevice(device)
	{
		mVtxBuffer = resource::buffer(
			mDevice, 
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			D3D12_RESOURCE_FLAG_NONE,
			D3D12_HEAP_TYPE_UPLOAD, 
			VtxBufferIncrease * sizeof(ImDrawVert));

		mIdxBuffer = resource::buffer(
			mDevice,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_FLAG_NONE,
			D3D12_HEAP_TYPE_UPLOAD,
			IdxBufferIncrease * sizeof(ImDrawIdx));

		mCurrentVtxBufferCount = VtxBufferIncrease;
		mCurrentIdxBufferCount = IdxBufferIncrease;
	}

	inline void imgui_frame_resources::update(ImDrawData* draw_data)
	{
		const auto old_vtx_buffer_count = mCurrentVtxBufferCount;
		const auto old_idx_buffer_count = mCurrentIdxBufferCount;
		
		while (mCurrentVtxBufferCount < draw_data->TotalVtxCount) mCurrentVtxBufferCount += VtxBufferIncrease;
		while (mCurrentIdxBufferCount < draw_data->TotalIdxCount) mCurrentIdxBufferCount += IdxBufferIncrease;

		if (mCurrentVtxBufferCount > old_vtx_buffer_count)
			mVtxBuffer = resource::buffer(
				mDevice,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_FLAG_NONE,
				D3D12_HEAP_TYPE_UPLOAD,
				mCurrentVtxBufferCount * sizeof(ImDrawVert)
			);

		if (mCurrentIdxBufferCount > old_idx_buffer_count)
			mIdxBuffer = resource::buffer(
				mDevice,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_FLAG_NONE,
				D3D12_HEAP_TYPE_UPLOAD,
				mCurrentIdxBufferCount * sizeof(ImDrawIdx)
			);

		auto vtx_memory = static_cast<ImDrawVert*>(mVtxBuffer.begin_mapping());
		auto idx_memory = static_cast<ImDrawIdx*>(mIdxBuffer.begin_mapping());
		
		for (auto index = 0; index < draw_data->CmdListsCount; index++) {
			const auto command_list = draw_data->CmdLists[index];

			std::memcpy(vtx_memory, command_list->VtxBuffer.Data, command_list->VtxBuffer.size_in_bytes());
			std::memcpy(idx_memory, command_list->IdxBuffer.Data, command_list->IdxBuffer.size_in_bytes());

			vtx_memory = vtx_memory + command_list->VtxBuffer.Size;
			idx_memory = idx_memory + command_list->IdxBuffer.Size;
		}

		mVtxBuffer.end_mapping();
		mIdxBuffer.end_mapping();
	}

	inline resource imgui_frame_resources::vtx_buffer() const
	{
		return mVtxBuffer;
	}

	inline resource imgui_frame_resources::idx_buffer() const
	{
		return mIdxBuffer;
	}

	inline void imgui_context::initialize(const device& device, const descriptor_heap& heap,
	                                      const D3D12_CPU_DESCRIPTOR_HANDLE& font_cpu_handle, 
	                                      const D3D12_GPU_DESCRIPTOR_HANDLE& font_gpu_handle,
	                                      const DXGI_FORMAT& render_target_view_format, 
	                                      size_t num_frames_in_flight)
	{
		auto& imgui_io = ImGui::GetIO();

		imgui_io.BackendRendererName = "imgui_directx12_wrapper";
		imgui_io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		Device = device;
		RenderTargetViewFormat = render_target_view_format;
		DescriptorHeap = heap;
		FontCpuHandle = font_cpu_handle;
		FontGpuHandle = font_gpu_handle;

		CurrentFrameIndex = std::numeric_limits<size_t>::max();

		for (size_t index = 0; index < num_frames_in_flight; index++)
			FrameResources.push_back(imgui_frame_resources(Device));
	}

	inline void imgui_context::new_frame()
	{
		if (!GraphicsPipeline.get()) create_pipeline_state();
	}

	inline void imgui_context::create_pipeline_state()
	{
		DescriptorTable = descriptor_table();

		DescriptorTable.add_srv_range({ "texture" }, 0, 0);

		RootSignatureInfo = root_signature_info();

		RootSignatureInfo
			.add_descriptor_table("table", DescriptorTable)
			.add_constants("view", 0, 0, 16)
			.add_static_sampler("sampler", 0, 0);

		RootSignature = root_signature::create(Device, RootSignatureInfo);

		static const char* vert_shader =
			"cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

		static const char* frag_shader =
			"struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            SamplerState sampler0 : register(s0);\
            Texture2D texture0 : register(t0);\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";

		VertShader = shader_code::create_from_source(vert_shader, "main", "vs_5_0");
		FragShader = shader_code::create_from_source(frag_shader, "main", "ps_5_0");
		
		input_assembly_info input_assembly;
		rasterization_info rasterization;
		depth_stencil_info depth_stencil;
		blend_info blend;
		
		input_assembly
			.add_input_element("POSITION", DXGI_FORMAT_R32G32_FLOAT)
			.add_input_element("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT)
			.add_input_element("COLOR", DXGI_FORMAT_R8G8B8A8_UNORM);

		rasterization
			.set_fill_mode(D3D12_FILL_MODE_SOLID)
			.set_cull_mode(D3D12_CULL_MODE_NONE);
		
		depth_stencil.set_depth_state(false);

		blend.set_render_target({
			true,
			false,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		}, 0);

		graphics_pipeline_info graphics_pipeline_info;

		graphics_pipeline_info
			.set_primitive_type(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
			.set_root_signature(RootSignature)
			.set_vert_shader(VertShader)
			.set_frag_shader(FragShader)
			.set_input_assembly(input_assembly)
			.set_rasterization(rasterization)
			.set_depth_stencil(depth_stencil)
			.set_blend(blend)
			.set_format({ RenderTargetViewFormat });

		GraphicsPipeline = pipeline_state::create(Device, graphics_pipeline_info);

		create_font_texture();
	}

	inline void imgui_context::create_font_texture()
	{
		
	}

}

#endif