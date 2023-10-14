#pragma once

#include "../descriptors/root_signature.hpp"
#include "../shaders/shader_library.hpp"

#include <optional>

namespace wrapper::directx12
{

	struct raytracing_shader_config
	{
		constexpr static inline size_t default_payload_size = 32;

		uint32 max_attribute_size = D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES;
		uint32 max_payload_size = default_payload_size;

		uint32 hash() const noexcept;

		static uint32 encode(const raytracing_shader_config& config);

		static raytracing_shader_config decode(uint32 code);
	};

	struct raytracing_shader_root_signature
	{
		root_signature signature;
		size_t size = 0;
	};

	struct raytracing_shader_association
	{
		std::optional<raytracing_shader_root_signature> root_signature;
		std::optional<raytracing_shader_config> config;
		
		std::wstring function;
	};

	struct raytracing_shader_record
	{
		size_t address = 0;
		size_t size = 0;
	};

	class raytracing_shader_table final
	{
	public:
		raytracing_shader_table() = default;
		
		raytracing_shader_table(
			const std::vector<raytracing_shader_association>& associations,
			const std::vector<std::wstring>& miss_shaders,
			const std::vector<std::wstring>& hit_groups,
			const std::wstring& ray_generation);

		byte* shader_record_address(const std::wstring& function);

		const std::unordered_map<std::wstring, raytracing_shader_record>& records() const noexcept;
		
		raytracing_shader_record ray_generation() const noexcept;

		raytracing_shader_record miss_shaders() const noexcept;

		raytracing_shader_record hit_groups() const noexcept;

		size_t size() const noexcept;
		
		const byte* data() const noexcept;

		byte* data() noexcept;

		static raytracing_shader_table create(
			const std::vector<raytracing_shader_association>& associations,
			const std::vector<std::wstring>& miss_shaders,
			const std::vector<std::wstring>& hit_groups,
			const std::wstring& ray_generation);
	private:
		std::unordered_map<std::wstring, raytracing_shader_record> mShaderRecords;

		std::vector<byte> mMemoryBuffer;
		
		raytracing_shader_record mRayGenerationShaderRecord;
		raytracing_shader_record mHitGroupShadersRecord;
		raytracing_shader_record mMissShadersRecord;
	};
}
