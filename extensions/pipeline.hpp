#pragma once

#define __DIRECTX_WRAPPER_EXTENSION_PIPELINE__
#ifndef __DIRECTX_WRAPPER_EXTENSION_PIPELINE__
#else

#include "../descriptors/root_signature.hpp"
#include "../pipelines/pipeline_state.hpp"
#include "../shaders/shader_code.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

namespace wrapper::directx12::extensions {

	struct render_pipeline final {
		root_signature_info root_signature_info;
		root_signature root_signature;

		input_assembly_info input_assembly;
		rasterization_info rasterization;
		depth_stencil_info depth_stencil;
		blend_info blend;

		shader_code vert_shader;
		shader_code frag_shader;

		graphics_pipeline_info graphics_pipeline_info;
		pipeline_state graphics_pipeline;

		std::vector<DXGI_FORMAT> rtv_format = {};

		DXGI_FORMAT dsv_format = DXGI_FORMAT_UNKNOWN;

		render_pipeline() = default;
	};

	class render_pipelines final {
	public:
		render_pipelines() = default;

		~render_pipelines() = default;
		
		bool empty() const noexcept;

		const render_pipeline& at(const std::string& name) const;

		render_pipeline& operator[](const std::string& name);

		static render_pipelines from_memory(const device& device, const nlohmann::json& memory);

		static render_pipelines from_file(const device& device, const std::string& filename);
	private:
		mapping<std::string, render_pipeline> mPipelines;
	private:
		using root_signature = std::pair<root_signature_info, root_signature>;
		
		template <typename Type>
		static const mapping<std::string, Type>& mapping_pool();

		template <typename Type>
		static Type from_string(const std::string& name);

		static descriptor_table read_descriptor_table(const nlohmann::json& descriptor_table);

		static mapping<std::string, root_signature> read_root_signatures(const device& device, const nlohmann::json& root_signatures);

		
		static void load_pipeline_shader(const nlohmann::json& shader, shader_code& code);

		static void load_pipeline_input_assembly(const nlohmann::json& input_assembly, render_pipeline& pipeline);

		static void load_pipeline_rasterization(const nlohmann::json& rasterization, render_pipeline& pipeline);

		static void load_pipeline_depth_stencil(const nlohmann::json& depth_stencil, render_pipeline& pipeline);
		
		static void load_pipeline_rtv_format(const nlohmann::json& format, render_pipeline& pipeline);
		
		static void load_pipeline_dsv_format(const nlohmann::json& format, render_pipeline& pipeline);

		
		static inline const mapping<std::string, std::function<void(descriptor_table&, const nlohmann::json&)>> TableEvents =
		{
			{"shader_resource_view",[](descriptor_table& table, const nlohmann::json& item)
			{
				table.add_srv_range(item["name"], item["index"], item["space"]);
			}},
			{"unordered_access_view",[](descriptor_table& table, const nlohmann::json& item)
			{
				table.add_uav_range(item["name"], item["index"], item["space"]);
			}}
		};

		static inline const mapping<std::string, std::function<void(root_signature&, const nlohmann::json&)>> RootSignatureEvents =
		{
			{"shader_resource_view",[](root_signature& r, const nlohmann::json& item)
			{
				r.first.add_shader_resource_view(item["name"], item["index"], item["space"]);
			}},
			{"constant",[](root_signature& r, const nlohmann::json& item)
			{
				r.first.add_constants(item["name"], item["index"], item["space"], item["count"]);
			}},
			{"descriptor_table",[](root_signature& r, const nlohmann::json& item)
			{
				r.first.add_descriptor_table(item["name"], read_descriptor_table(item));
			}},
			{"static_sampler",[](root_signature& r, const nlohmann::json& item)
			{
				r.first.add_static_sampler(item["name"], item["index"], item["space"]);
			}}
		};
		
		static inline const mapping<std::string, D3D12_FILL_MODE> FillModePool =
		{
			{ "solid", D3D12_FILL_MODE_SOLID },
			{ "wireframe", D3D12_FILL_MODE_WIREFRAME }
		};

		static inline const mapping<std::string, D3D12_CULL_MODE> CullModePool =
		{
			{ "front", D3D12_CULL_MODE_FRONT },
			{ "back", D3D12_CULL_MODE_BACK },
			{ "none", D3D12_CULL_MODE_NONE }
		};

		static inline const mapping<std::string, DXGI_FORMAT> FormatPool =
		{
			{ "UNKNOWN", DXGI_FORMAT_UNKNOWN },
			{ "R32G32B32FLOAT", DXGI_FORMAT_R32G32B32_FLOAT },
			{ "R8G8B8A8UNORM", DXGI_FORMAT_R8G8B8A8_UNORM },
			{ "D32FLOAT", DXGI_FORMAT_D32_FLOAT },
			{ "R32UINT", DXGI_FORMAT_R32_UINT }
		};
	};
	
	template <>
	inline const mapping<std::string, DXGI_FORMAT>& render_pipelines::mapping_pool()
	{
		return FormatPool;
	}

	template <>
	inline const mapping<std::string, D3D12_FILL_MODE>& render_pipelines::mapping_pool()
	{
		return FillModePool;
	}

	template <>
	inline const mapping<std::string, D3D12_CULL_MODE>& render_pipelines::mapping_pool()
	{
		return CullModePool;
	}
	
	template <typename Type>
	Type render_pipelines::from_string(const std::string& name)
	{
		return mapping_pool<Type>().at(name);
	}

	inline bool render_pipelines::empty() const noexcept
	{
		return mPipelines.empty();
	}

	inline const render_pipeline& render_pipelines::at(const std::string& name) const
	{
		return mPipelines.at(name);
	}

	inline render_pipeline& render_pipelines::operator[](const std::string& name)
	{
		return mPipelines[name];
	}

	inline render_pipelines render_pipelines::from_memory(const device& device, const nlohmann::json& memory)
	{
		render_pipelines pipelines;

		const auto root_signatures = read_root_signatures(device, memory["root_signatures"]);

		for (const auto& pipeline : memory["pipelines"]) {
			const auto name = pipeline["name"];

			load_pipeline_input_assembly(pipeline["input_assembly"], pipelines[name]);
			load_pipeline_rasterization(pipeline["rasterization"], pipelines[name]);
			load_pipeline_depth_stencil(pipeline["depth_stencil"], pipelines[name]);
			load_pipeline_rtv_format(pipeline["rtv_format"], pipelines[name]);
			load_pipeline_dsv_format(pipeline["dsv_format"], pipelines[name]);
			load_pipeline_shader(pipeline["vert_shader"], pipelines[name].vert_shader);
			load_pipeline_shader(pipeline["frag_shader"], pipelines[name].frag_shader);

			const auto [root_signature_info, root_signature] = root_signatures.at(pipeline["root_signature"]);

			pipelines[name].root_signature_info = root_signature_info;
			pipelines[name].root_signature = root_signature;

			pipelines[name].graphics_pipeline_info
				.set_primitive_type(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
				.set_root_signature(pipelines[name].root_signature)
				.set_vert_shader(pipelines[name].vert_shader)
				.set_frag_shader(pipelines[name].frag_shader)
				.set_input_assembly(pipelines[name].input_assembly)
				.set_rasterization(pipelines[name].rasterization)
				.set_depth_stencil(pipelines[name].depth_stencil)
				.set_blend(pipelines[name].blend)
				.set_format(pipelines[name].rtv_format, pipelines[name].dsv_format);

			pipelines[name].graphics_pipeline = pipeline_state::create(
				device, pipelines[name].graphics_pipeline_info);

		}

		return pipelines;
	}

	inline render_pipelines render_pipelines::from_file(const device& device, const std::string& filename)
	{
		std::ifstream stream(filename);

		nlohmann::json memory; stream >> memory; stream.close();

		return from_memory(device, memory);
	}

	inline descriptor_table render_pipelines::read_descriptor_table(const nlohmann::json& descriptor_table)
	{
		directx12::descriptor_table table;

		for (const auto& range : descriptor_table["ranges"]) 
			TableEvents.at(range["type"])(table, range);

		return table;
	}

	inline mapping<std::string, render_pipelines::root_signature> render_pipelines::read_root_signatures(
		const device& device, const nlohmann::json& root_signatures)
	{
		mapping<std::string, root_signature> root_signature_mapping;

		for (const auto& root_signature : root_signatures) {
			const auto name = root_signature["name"];

			for (const auto& descriptor : root_signature["descriptors"])
				RootSignatureEvents.at(descriptor["type"])(root_signature_mapping[name], descriptor);

			root_signature_mapping[name].second = directx12::root_signature::create(
				device, root_signature_mapping[name].first);
		}

		return root_signature_mapping;
	}

	inline void render_pipelines::load_pipeline_shader(const nlohmann::json& shader, shader_code& code)
	{
		std::vector<std::pair<std::string, std::string>> macros;

		if (shader.contains("macros")) {
			for (const auto& macro : shader["macros"])
				macros.push_back({ macro["name"], macro["value"] });
		}

		code = shader_code::create_from_file(
			shader["filepath"].get<std::string>(), shader["entry"], shader["version"], macros);
	}

	inline void render_pipelines::load_pipeline_input_assembly(const nlohmann::json& input_assembly, render_pipeline& pipeline)
	{
		for (const auto& item : input_assembly)
			pipeline.input_assembly.add_input_element(item["name"], from_string<DXGI_FORMAT>(item["format"]), item["slot"]);
	}

	inline void render_pipelines::load_pipeline_rasterization(const nlohmann::json& rasterization, render_pipeline& pipeline)
	{
		if (rasterization.contains("fill_mode"))
			pipeline.rasterization.set_fill_mode(from_string<D3D12_FILL_MODE>(rasterization["fill_mode"]));

		if (rasterization.contains("cull_mode"))
			pipeline.rasterization.set_cull_mode(from_string<D3D12_CULL_MODE>(rasterization["cull_mode"]));
	}

	inline void render_pipelines::load_pipeline_depth_stencil(const nlohmann::json& depth_stencil, render_pipeline& pipeline)
	{
		if (depth_stencil.contains("enable"))
			pipeline.depth_stencil.set_depth_enable(depth_stencil["enable"]);
	}

	inline void render_pipelines::load_pipeline_rtv_format(const nlohmann::json& format, render_pipeline& pipeline)
	{
		for (const auto& item : format)
			pipeline.rtv_format.push_back(from_string<DXGI_FORMAT>(item));
	}

	inline void render_pipelines::load_pipeline_dsv_format(const nlohmann::json& format, render_pipeline& pipeline)
	{
		pipeline.dsv_format = from_string<DXGI_FORMAT>(format.get<std::string>());
	}

}

#endif