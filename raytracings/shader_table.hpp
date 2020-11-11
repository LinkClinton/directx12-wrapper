#pragma once

#include "../descriptors/root_signature.hpp"

#include <optional>

namespace wrapper::directx12 {

	struct raytracing_shader_config {
		constexpr static inline size_t default_payload_size = 32;

		size_t max_attribute_size = D3D12_RAYTRACING_MAX_ATTRIBUTE_SIZE_IN_BYTES;
		size_t max_payload_size = default_payload_size;

		raytracing_shader_config() = default;

		size_t hash() const noexcept;

		static size_t encode(const raytracing_shader_config& config);

		static raytracing_shader_config decode(size_t code);
	};

	struct raytracing_shader_root_signature {
		root_signature signature;
		size_t size = 0;

		raytracing_shader_root_signature() = default;
	};

	struct raytracing_shader_association {
		std::optional<raytracing_shader_root_signature> root_signature;
		std::optional<raytracing_shader_config> config;
		
		std::wstring function;

		raytracing_shader_association() = default;
	};

	struct raytracing_shader_record {
		size_t address = 0;
		size_t size = 0;

		raytracing_shader_record() = default;
	};

	class raytracing_shader_table final {
	public:
		raytracing_shader_table(
			const std::vector<raytracing_shader_association>& associations,
			const std::vector<std::wstring>& miss_shaders,
			const std::vector<std::wstring>& hit_groups,
			const std::wstring& ray_generation);

		~raytracing_shader_table() = default;

		byte* shader_record_address(const std::wstring& function);

		raytracing_shader_record ray_generation() const noexcept;

		raytracing_shader_record miss_shaders() const noexcept;

		raytracing_shader_record hit_groups() const noexcept;

		size_t size() const noexcept;
		
		const byte* data() const noexcept;
	private:
		std::unordered_map<std::wstring, raytracing_shader_record> mShaderRecords;

		std::vector<byte> mMemoryBuffer;
		
		raytracing_shader_record mRayGenerationShaderRecord;
		raytracing_shader_record mHitGroupShadersRecord;
		raytracing_shader_record mMissShadersRecord;
	};
}
