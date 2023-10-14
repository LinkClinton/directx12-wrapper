#pragma once

#include "../interfaces/wrapper.hpp"
#include "adapter.hpp"

namespace wrapper::directx12
{

	class resource;
	
	class device final : public wrapper_t<ID3D12Device5>
	{
	public:
		device() = default;

		explicit device(const ComPtr<ID3D12Device5>& source);
		
		void create_render_target_view(
			const D3D12_RENDER_TARGET_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle,
			const resource& resource) const;

		void create_depth_stencil_view(
			const D3D12_DEPTH_STENCIL_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle,
			const resource& resource) const;

		void create_shader_resource_view(
			const D3D12_SHADER_RESOURCE_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle,
			const resource& resource) const;

		void create_shader_resource_view(
			const D3D12_SHADER_RESOURCE_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle) const;
		
		void create_constant_buffer_view(
			const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle) const;

		void create_unordered_access_view(
			const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc,
			const D3D12_CPU_DESCRIPTOR_HANDLE& handle,
			const resource& resource) const;
		
		static device create(const D3D_FEATURE_LEVEL& level);

		static device create(const adapter& adapter, const D3D_FEATURE_LEVEL& level);
	};
	
}
