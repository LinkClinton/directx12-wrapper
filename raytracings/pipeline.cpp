#include "pipeline.hpp"

namespace wrapper::directx12
{

	LPCWSTR get_import(const std::wstring& str) { return str.empty() ? nullptr : str.c_str(); }
	
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_raytracing_shader_associations(
	const std::vector<raytracing_shader_association>& associations)
{
	mAssociations = associations;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_raytracing_hit_groups(
	const std::vector<raytracing_hit_group>& hit_groups)
{
	mHitGroups = hit_groups;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_shader_libraries(
	const std::vector<shader_library>& libraries)
{
	mLibraries = libraries;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_miss_shaders(
	const std::vector<std::wstring>& shaders)
{
	mMissShaders = shaders;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_ray_generation(
	const std::wstring& shader)
{
	mRayGeneration = shader;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_root_signature(
	const root_signature& signature)
{
	mRootSignature = signature;

	return *this;
}

wrapper::directx12::raytracing_pipeline_info& wrapper::directx12::raytracing_pipeline_info::set_max_depth(size_t depth)
{
	mMaxDepth = depth;

	return *this;
}

const std::vector<wrapper::directx12::raytracing_shader_association>& 
	wrapper::directx12::raytracing_pipeline_info::associations() const noexcept
{
	return mAssociations;
}

const std::vector<wrapper::directx12::raytracing_hit_group>& wrapper::directx12::raytracing_pipeline_info::hit_groups() const noexcept
{
	return mHitGroups;
}

const std::vector<wrapper::directx12::shader_library>& wrapper::directx12::raytracing_pipeline_info::libraries() const noexcept
{
	return mLibraries;
}

const std::vector<std::wstring>& wrapper::directx12::raytracing_pipeline_info::miss_shaders() const noexcept
{
	return mMissShaders;
}

const std::wstring& wrapper::directx12::raytracing_pipeline_info::ray_generation() const noexcept
{
	return mRayGeneration;
}

const wrapper::directx12::root_signature& wrapper::directx12::raytracing_pipeline_info::signature() const noexcept
{
	return mRootSignature;
}

size_t wrapper::directx12::raytracing_pipeline_info::max_depth() const noexcept
{
	return mMaxDepth;
}

wrapper::directx12::raytracing_pipeline::raytracing_pipeline(const ComPtr<ID3D12StateObject>& pipeline) :
	wrapper_t<ID3D12StateObject>(pipeline)
{
	mWrapperInstance->QueryInterface(IID_PPV_ARGS(mProperties.GetAddressOf()));
}

void* wrapper::directx12::raytracing_pipeline::shader_identifier(const std::wstring& name) const
{
	return mProperties->GetShaderIdentifier(name.c_str());
}

wrapper::directx12::raytracing_pipeline wrapper::directx12::raytracing_pipeline::create(
	const device& device, const raytracing_pipeline_info& info)
{
	using shader_functions = std::vector<LPCWSTR>;

	std::unordered_map<ID3D12RootSignature*, shader_functions> signature_associations;
	std::unordered_map<uint32, shader_functions> config_associations;

	for (const auto& association : info.associations()) 
	{
		if (association.root_signature.has_value())
			signature_associations[association.root_signature->signature.get()].push_back(association.function.c_str());

		if (association.config.has_value())
			config_associations[association.config->hash()].push_back(association.function.c_str());
	}

	std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION> associations(
		signature_associations.size() + config_associations.size()
	);

	std::vector<D3D12_STATE_SUBOBJECT> subobjects(
		signature_associations.size() * 2 + config_associations.size() * 2 +
		info.libraries().size() + info.hit_groups().size() + 1 +
		(info.signature().get() == nullptr ? 0 : 1)
	);

	size_t association_count = 0;
	size_t subobject_count = 0;

	std::vector<D3D12_RAYTRACING_SHADER_CONFIG> configs(config_associations.size());

	size_t config_index = 0;
	
	for (auto& config_association : config_associations)
	{
		const auto config = raytracing_shader_config::decode(config_association.first);

		auto& association = associations[association_count++];
		auto& subobject_config = subobjects[subobject_count++];
		auto& subobject_export = subobjects[subobject_count++];

		configs[config_index].MaxAttributeSizeInBytes = config.max_attribute_size;
		configs[config_index].MaxPayloadSizeInBytes = config.max_payload_size;

		subobject_config.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
		subobject_config.pDesc = &configs[config_index++];

		association.NumExports = static_cast<UINT>(config_association.second.size());
		association.pSubobjectToAssociate = &subobject_config;
		association.pExports = config_association.second.data();

		subobject_export.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
		subobject_export.pDesc = &association;
	}

	std::vector<D3D12_LOCAL_ROOT_SIGNATURE> local_signatures(signature_associations.size());

	size_t signature_index = 0;

	for (auto& signature_association : signature_associations)
	{
		auto& association = associations[association_count++];
		auto& subobject_signature = subobjects[subobject_count++];
		auto& subobject_export = subobjects[subobject_count++];

		local_signatures[signature_index].pLocalRootSignature = signature_association.first;

		subobject_signature.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
		subobject_signature.pDesc = &local_signatures[signature_index++];

		association.NumExports = static_cast<UINT>(signature_association.second.size());
		association.pSubobjectToAssociate = &subobject_signature;
		association.pExports = signature_association.second.data();

		subobject_export.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
		subobject_export.pDesc = &association;
	}

	std::vector<D3D12_DXIL_LIBRARY_DESC> libraries(info.libraries().size());

	for (size_t index = 0; index < info.libraries().size(); index++) 
	{
		auto& subobject = subobjects[subobject_count++];

		libraries[index] = info.libraries()[index].desc();

		subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
		subobject.pDesc = &libraries[index];
	}

	std::vector<D3D12_HIT_GROUP_DESC> hit_groups(info.hit_groups().size());

	for (size_t index = 0; index < info.hit_groups().size(); index++)
	{
		auto& subobject = subobjects[subobject_count++];
		
		hit_groups[index].Type = info.hit_groups()[index].type;
		hit_groups[index].IntersectionShaderImport = get_import(info.hit_groups()[index].intersection);
		hit_groups[index].ClosestHitShaderImport = get_import(info.hit_groups()[index].closest);
		hit_groups[index].AnyHitShaderImport = get_import(info.hit_groups()[index].any);
		hit_groups[index].HitGroupExport = get_import(info.hit_groups()[index].name);

		subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
		subobject.pDesc = &hit_groups[index];
	}

	D3D12_GLOBAL_ROOT_SIGNATURE global_root_signature = {};
	
	if (info.signature().get() != nullptr)
	{
		auto& subobject = subobjects[subobject_count++];

		global_root_signature.pGlobalRootSignature = info.signature().get();

		subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
		subobject.pDesc = &global_root_signature;
	}

	D3D12_RAYTRACING_PIPELINE_CONFIG config;
	{
		auto& subobject = subobjects[subobject_count++];

		config.MaxTraceRecursionDepth = static_cast<UINT>(info.max_depth());

		subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
		subobject.pDesc = &config;
	}

	assert(association_count == associations.size());
	assert(subobject_count == subobjects.size());

	D3D12_STATE_OBJECT_DESC desc;

	desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
	desc.NumSubobjects = static_cast<UINT>(subobjects.size());
	desc.pSubobjects = subobjects.data();

	ComPtr<ID3D12StateObject> state_object;
	
	device->CreateStateObject(&desc, IID_PPV_ARGS(state_object.GetAddressOf()));

	return raytracing_pipeline(state_object);
}