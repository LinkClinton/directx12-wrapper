#pragma once

#include "../device.hpp"

#include <vector>

namespace wrapper::directx12 {
	
	class root_signature_info final {
	public:
		root_signature_info() = default;

		~root_signature_info() = default;

		const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers() const;

		const std::vector<D3D12_ROOT_PARAMETER>& parameters() const;
	private:
		std::vector<D3D12_STATIC_SAMPLER_DESC> mStaticSamplers;
		std::vector<D3D12_ROOT_PARAMETER> mRootParameters;
	};
	
	class root_signature final : public wrapper_t<ID3D12RootSignature> {
	public:
		root_signature() = default;

		root_signature(const ComPtr<ID3D12RootSignature>& source);
		
		~root_signature() = default;

		static root_signature create(const device& device, const root_signature_info& info, bool local = false);
	};
	
}
