#pragma once

#include "shader_table.hpp"

namespace wrapper::directx12
{

	struct raytracing_hit_group
	{
		D3D12_HIT_GROUP_TYPE type;

		std::wstring intersection;
		std::wstring closest;
		std::wstring any;
		
		std::wstring name;
	};
	
	class raytracing_pipeline_info final
	{
	public:
		raytracing_pipeline_info& set_raytracing_shader_associations(const std::vector<raytracing_shader_association>& associations);
		
		raytracing_pipeline_info& set_raytracing_hit_groups(const std::vector<raytracing_hit_group>& hit_groups);

		raytracing_pipeline_info& set_shader_libraries(const std::vector<shader_library>& libraries);

		raytracing_pipeline_info& set_miss_shaders(const std::vector<std::wstring>& shaders);

		raytracing_pipeline_info& set_ray_generation(const std::wstring& shader);

		raytracing_pipeline_info& set_root_signature(const root_signature& signature);

		raytracing_pipeline_info& set_max_depth(size_t depth);
		
		const std::vector<raytracing_shader_association>& associations() const noexcept;

		const std::vector<raytracing_hit_group>& hit_groups() const noexcept;

		const std::vector<shader_library>& libraries() const noexcept;

		const std::vector<std::wstring>& miss_shaders() const noexcept;

		const std::wstring& ray_generation() const noexcept;

		const root_signature& signature() const noexcept;

		size_t max_depth() const noexcept;
	private:
		std::vector<raytracing_shader_association> mAssociations;
		std::vector<raytracing_hit_group> mHitGroups;
		
		std::vector<shader_library> mLibraries;

		std::vector<std::wstring> mMissShaders;
		std::wstring mRayGeneration;

		root_signature mRootSignature;

		size_t mMaxDepth = 5;
	};

	class raytracing_pipeline final : public wrapper_t<ID3D12StateObject>
	{
	public:
		raytracing_pipeline() = default;

		raytracing_pipeline(const ComPtr<ID3D12StateObject>& pipeline);

		void* shader_identifier(const std::wstring& name) const;
		
		static raytracing_pipeline create(const device& device, const raytracing_pipeline_info& info);
	private:
		ComPtr<ID3D12StateObjectProperties> mProperties;
	};
}
