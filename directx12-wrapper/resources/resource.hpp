#pragma once

#include "../commands/command_list.hpp"
#include "../devices/device.hpp"

namespace wrapper::directx12 {

	class texture2d;
	class buffer;
	
	class resource_info final {
	public:
		resource_info() = default;

		explicit resource_info(
			const D3D12_RESOURCE_STATES& state,
			const D3D12_RESOURCE_FLAGS& flags,
			const D3D12_HEAP_TYPE& type);
		
		~resource_info() = default;

		resource_info& set_resource_state(const D3D12_RESOURCE_STATES& state);

		resource_info& set_resource_flags(const D3D12_RESOURCE_FLAGS& flags);

		resource_info& set_heap_type(const D3D12_HEAP_TYPE& type);

		D3D12_RESOURCE_STATES state() const;

		D3D12_RESOURCE_FLAGS flags() const;

		D3D12_HEAP_TYPE type() const;

		static resource_info upload(
			const D3D12_RESOURCE_STATES& state = D3D12_RESOURCE_STATE_GENERIC_READ,
			const D3D12_RESOURCE_FLAGS& flags = D3D12_RESOURCE_FLAG_NONE);

		static resource_info common(
			const D3D12_RESOURCE_STATES& state = D3D12_RESOURCE_STATE_GENERIC_READ,
			const D3D12_RESOURCE_FLAGS& flags = D3D12_RESOURCE_FLAG_NONE);
	private:
		D3D12_RESOURCE_STATES mState;
		D3D12_RESOURCE_FLAGS mFlags;
		D3D12_HEAP_TYPE mHeapType;
	};

	class resource_view final {
	public:
		resource_view() = delete;

		~resource_view() = default;

		static D3D12_VERTEX_BUFFER_VIEW vertex_buffer(const buffer& buffer, size_t stride_in_bytes, size_t size_in_bytes);

		static D3D12_INDEX_BUFFER_VIEW index_buffer(const buffer& buffer, const DXGI_FORMAT& format, size_t size_in_bytes);

		static D3D12_RENDER_TARGET_VIEW_DESC render_target2d(const DXGI_FORMAT& format, uint32 mip_slice = 0);
		
		template <typename T>
		static D3D12_VERTEX_BUFFER_VIEW vertex_buffer(const buffer& buffer, size_t count);

		template <typename T>
		static D3D12_INDEX_BUFFER_VIEW index_buffer(const buffer& buffer, size_t count);
	};

	template <typename T>
	D3D12_VERTEX_BUFFER_VIEW resource_view::vertex_buffer(const buffer& buffer, size_t count)
	{
		return vertex_buffer(buffer, sizeof(T), count * sizeof(T));
	}

	template <typename T>
	D3D12_INDEX_BUFFER_VIEW resource_view::index_buffer(const buffer& buffer, size_t count)
	{
		static_assert(sizeof(T) == 4 || sizeof(T) == 2);
		
		return index_buffer(buffer, sizeof(T) == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, count * sizeof(T));
	}

	class resource : public wrapper_t<ID3D12Resource> {
	public:
		resource() = default;

		explicit resource(const ComPtr<ID3D12Resource>& source);
		
		~resource() = default;

		auto barrier(
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const
			->D3D12_RESOURCE_BARRIER;

		void barrier(
			const graphics_command_list& command_list,
			const D3D12_RESOURCE_STATES& before,
			const D3D12_RESOURCE_STATES& after) const;
	protected:
		D3D12_RESOURCE_DESC mDesc;

		D3D12_HEAP_PROPERTIES mHeapProperties;
		D3D12_HEAP_FLAGS mHeapFlags;
	};
}
