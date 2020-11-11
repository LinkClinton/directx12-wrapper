#include "shader_table.hpp"

size_t wrapper::directx12::raytracing_shader_config::hash() const noexcept
{
	assert(max_attribute_size <= D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES);

	return encode(*this);
}

size_t wrapper::directx12::raytracing_shader_config::encode(const raytracing_shader_config& config)
{
	return config.max_payload_size * D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES + config.max_attribute_size;
}

wrapper::directx12::raytracing_shader_config wrapper::directx12::raytracing_shader_config::decode(size_t code)
{
	return raytracing_shader_config{
		code % D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES,
		code / D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES
	};
}

wrapper::directx12::raytracing_shader_table::raytracing_shader_table(
	const std::vector<raytracing_shader_association>& associations, 
	const std::vector<std::wstring>& miss_shaders,
	const std::vector<std::wstring>& hit_groups,
	const std::wstring& ray_generation)
{
	// first, we will loop associations to build shader record with root signature
	// the size of shader record is not the final size
	// because the size of different hit groups or miss shaders should be same
	for (const auto& association : associations) {
		if (!association.root_signature.has_value()) continue;

		const auto shader_record_size = align_to(
			D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + association.root_signature->size,
			D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

		mShaderRecords.insert({ association.function, raytracing_shader_record{0, shader_record_size} });
	}

	const auto shader_record_size_without_signature = align_to(D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
		D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

	mRayGenerationShaderRecord.address = 0;

	// find max size of shader record for each shader type(ray generation, hit group, miss shader)
	// for ray generation, if it has root signature we can find the size from mShaderRecords
	// otherwise, the size should be "shader_record_size_without_signature"
	if (mShaderRecords.find(ray_generation) != mShaderRecords.end())
		mRayGenerationShaderRecord.size = mShaderRecords[ray_generation].size;
	else
		mRayGenerationShaderRecord.size = shader_record_size_without_signature;

	for (const auto& miss_shader : miss_shaders) {
		const auto shader_record_size = mShaderRecords.find(miss_shader) != mShaderRecords.end() ?
			mShaderRecords[miss_shader].size : shader_record_size_without_signature;

		mMissShadersRecord.size = std::max(mMissShadersRecord.size, shader_record_size);
	}

	for (const auto& hit_group : hit_groups) {
		const auto shader_record_size = mShaderRecords.find(hit_group) != mShaderRecords.end() ?
			mShaderRecords[hit_group].size : shader_record_size_without_signature;

		mHitGroupShadersRecord.size = std::max(mHitGroupShadersRecord.size, shader_record_size);
	}

	// if ray generation does not have root signature we will add it into mShaderRecords
	if (mShaderRecords.find(ray_generation) == mShaderRecords.end())
		mShaderRecords.insert({ ray_generation, mRayGenerationShaderRecord });

	size_t shader_record_offset = align_to(mRayGenerationShaderRecord.address + mRayGenerationShaderRecord.size,
		D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);

	mMissShadersRecord.address = shader_record_offset;

	for (const auto& miss_shader : miss_shaders) {
		mShaderRecords[miss_shader] = raytracing_shader_record{ shader_record_offset, mMissShadersRecord.size };

		shader_record_offset += mMissShadersRecord.size;
	}

	shader_record_offset = align_to(shader_record_offset, D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);

	mHitGroupShadersRecord.address = shader_record_offset;

	for (const auto& hit_group : hit_groups) {
		mShaderRecords[hit_group] = raytracing_shader_record{ shader_record_offset, mHitGroupShadersRecord.size };

		shader_record_offset += mHitGroupShadersRecord.size;
	}

	mMemoryBuffer = std::vector<byte>(shader_record_offset);
}

byte* wrapper::directx12::raytracing_shader_table::shader_record_address(const std::wstring& function)
{
	return mMemoryBuffer.data() + mShaderRecords.at(function).address;
}

wrapper::directx12::raytracing_shader_record wrapper::directx12::raytracing_shader_table::ray_generation() const noexcept
{
	return mRayGenerationShaderRecord;
}

wrapper::directx12::raytracing_shader_record wrapper::directx12::raytracing_shader_table::miss_shaders() const noexcept
{
	return mMissShadersRecord;
}

wrapper::directx12::raytracing_shader_record wrapper::directx12::raytracing_shader_table::hit_groups() const noexcept
{
	return mHitGroupShadersRecord;
}

size_t wrapper::directx12::raytracing_shader_table::size() const noexcept
{
	return mMemoryBuffer.size();
}

const byte* wrapper::directx12::raytracing_shader_table::data() const noexcept
{
	return mMemoryBuffer.data();
}
