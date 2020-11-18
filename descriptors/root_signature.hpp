#pragma once

#include "descriptor_heap.hpp"
#include "../devices/device.hpp"

#include <vector>
#include <string>

namespace wrapper::directx12 {
	
	class root_signature_info final {
	public:
		root_signature_info() = default;

		~root_signature_info() = default;

		root_signature_info& add_descriptor(const std::string& name, const D3D12_ROOT_PARAMETER_TYPE& type, uint32 base, uint32 space);

		root_signature_info& add_descriptor_table(const std::string& name, const descriptor_table& table);

		root_signature_info& add_constants(const std::string& name, uint32 base, uint32 space, uint32 count);

		root_signature_info& add_unordered_access_view(const std::string& name, uint32 base, uint32 space);
		
		root_signature_info& add_shader_resource_view(const std::string& name, uint32 base, uint32 space);

		root_signature_info& add_constant_buffer_view(const std::string& name, uint32 base, uint32 space);

		root_signature_info& add_static_sampler(const std::string& name, uint32 base, uint32 space);
		
		uint32 index(const std::string& name) const;
		
		size_t base(const std::string& name) const;

		size_t size(const std::string& name) const;

		size_t size() const;

		const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers() const;

		const std::vector<D3D12_ROOT_PARAMETER>& parameters() const;
	private:
		root_signature_info& add_root_parameter(const std::string& name, const D3D12_ROOT_PARAMETER& parameter);
		
		std::vector<D3D12_STATIC_SAMPLER_DESC> mStaticSamplers;
		std::vector<D3D12_ROOT_PARAMETER> mRootParameters;

		std::unordered_map<std::string, uint32> mDescriptorIndex;

		std::vector<size_t> mDescriptorBase;
		std::vector<size_t> mDescriptorSize;

		size_t mSize = 0;
	};
	
	class root_signature final : public wrapper_t<ID3D12RootSignature> {
	public:
		root_signature() = default;

		root_signature(const ComPtr<ID3D12RootSignature>& source);
		
		~root_signature() = default;

		static root_signature create(const device& device, const root_signature_info& info, bool local = false);
	};
	
}
