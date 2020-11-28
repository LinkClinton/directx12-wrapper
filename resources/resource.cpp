#include "resource.hpp"

#include "../raytracings/accleration.hpp"
#include "buffer.hpp"

#include <memory>

wrapper::directx12::resource_info::resource_info(
	const D3D12_RESOURCE_STATES& state, const D3D12_RESOURCE_FLAGS& flags,
	const D3D12_HEAP_TYPE& type) :
	mState(state), mFlags(flags), mHeapType(type)
{
}

wrapper::directx12::resource_info& wrapper::directx12::resource_info::set_resource_state(
	const D3D12_RESOURCE_STATES& state)
{
	mState = state;
	
	return *this;
}

wrapper::directx12::resource_info& wrapper::directx12::resource_info::set_resource_flags(
	const D3D12_RESOURCE_FLAGS& flags)
{
	mFlags = flags;

	return *this;
}

wrapper::directx12::resource_info& wrapper::directx12::resource_info::set_heap_type(const D3D12_HEAP_TYPE& type)
{
	mHeapType = type;

	return *this;
}

D3D12_RESOURCE_STATES wrapper::directx12::resource_info::state() const
{
	return mState;
}

D3D12_RESOURCE_FLAGS wrapper::directx12::resource_info::flags() const
{
	return mFlags;
}

D3D12_HEAP_TYPE wrapper::directx12::resource_info::type() const
{
	return mHeapType;
}

wrapper::directx12::resource_info wrapper::directx12::resource_info::upload(const D3D12_RESOURCE_STATES& state, const D3D12_RESOURCE_FLAGS& flags)
{
	return resource_info(state, flags, D3D12_HEAP_TYPE_UPLOAD);
}

wrapper::directx12::resource_info wrapper::directx12::resource_info::common(const D3D12_RESOURCE_STATES& state, const D3D12_RESOURCE_FLAGS& flags)
{
	return resource_info(state, flags, D3D12_HEAP_TYPE_DEFAULT);
}

wrapper::directx12::resource_info wrapper::directx12::resource_info::readback(const D3D12_RESOURCE_STATES& state, const D3D12_RESOURCE_FLAGS& flags)
{
	return resource_info(state, flags, D3D12_HEAP_TYPE_READBACK);
}

D3D12_VERTEX_BUFFER_VIEW wrapper::directx12::resource_view::vertex_buffer(const buffer& buffer, size_t stride_in_bytes, size_t size_in_bytes)
{
	return {
		buffer->GetGPUVirtualAddress(),
		static_cast<UINT>(size_in_bytes),
		static_cast<UINT>(stride_in_bytes)
	};
}

D3D12_INDEX_BUFFER_VIEW wrapper::directx12::resource_view::index_buffer(const buffer& buffer, const DXGI_FORMAT& format, size_t size_in_bytes)
{
	return {
		buffer->GetGPUVirtualAddress(),
		static_cast<UINT>(size_in_bytes),
		format
	};
}

D3D12_CONSTANT_BUFFER_VIEW_DESC wrapper::directx12::resource_view::constant_buffer(const buffer& buffer, size_t offset,
	size_t size_in_bytes)
{
	return {
		buffer->GetGPUVirtualAddress() + offset,
		static_cast<UINT>(size_in_bytes)
	};
}

D3D12_CONSTANT_BUFFER_VIEW_DESC wrapper::directx12::resource_view::constant_buffer(const buffer& buffer)
{
	return {
		buffer->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer.size_in_bytes())
	};
}

D3D12_RENDER_TARGET_VIEW_DESC wrapper::directx12::resource_view::render_target2d(const DXGI_FORMAT& format, uint32 mip_slice)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc;

	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = format;
	
	desc.Texture2D.PlaneSlice = 0;
	desc.Texture2D.MipSlice = mip_slice;
	
	return desc;
}

D3D12_DEPTH_STENCIL_VIEW_DESC wrapper::directx12::resource_view::depth_stencil2d(const DXGI_FORMAT& format, uint32 mip_slice)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc;

	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Format = format;
	desc.Flags = D3D12_DSV_FLAG_NONE;
	
	desc.Texture2D.MipSlice = mip_slice;

	return desc;
}

D3D12_SHADER_RESOURCE_VIEW_DESC wrapper::directx12::resource_view::structured_buffer(size_t stride_in_bytes, size_t size_in_bytes)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc;

	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;

	desc.Buffer.FirstElement = 0;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Buffer.NumElements = static_cast<UINT>(size_in_bytes / stride_in_bytes);
	desc.Buffer.StructureByteStride = static_cast<UINT>(stride_in_bytes);

	return desc;
}

D3D12_SHADER_RESOURCE_VIEW_DESC wrapper::directx12::resource_view::acceleration(
	const raytracing_acceleration& acceleration)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc;

	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	
	desc.RaytracingAccelerationStructure.Location = acceleration.acceleration()->GetGPUVirtualAddress();

	return desc;
}

D3D12_SHADER_RESOURCE_VIEW_DESC wrapper::directx12::resource_view::texture2d(const DXGI_FORMAT& format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc;

	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Format = format;

	// https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_tex2d_srv
	
	desc.Texture2D.PlaneSlice = 0;
	desc.Texture2D.MipLevels = -1; // Set to -1 to indicate all the mipmap levels from MostDetailedMip on down to least detailed.
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.ResourceMinLODClamp = 0.f;
	
	return desc;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC wrapper::directx12::resource_view::read_write_texture2d(const DXGI_FORMAT& format)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc;

	desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	desc.Format = format;
	desc.Texture2D.MipSlice = 0;
	desc.Texture2D.PlaneSlice = 0;

	return desc;
}

wrapper::directx12::resource::resource(const ComPtr<ID3D12Resource>& source) : wrapper_t<ID3D12Resource>(source)
{
	mDesc = mWrapperInstance->GetDesc();

	mWrapperInstance->GetHeapProperties(&mHeapProperties, &mHeapFlags);
}

auto wrapper::directx12::resource::barrier(const D3D12_RESOURCE_STATES& before, const D3D12_RESOURCE_STATES& after) const
	-> D3D12_RESOURCE_BARRIER
{
	D3D12_RESOURCE_BARRIER barrier;

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.pResource = mWrapperInstance.Get();

	return barrier;
}

void wrapper::directx12::resource::barrier(const graphics_command_list& command_list,
	const D3D12_RESOURCE_STATES& before, const D3D12_RESOURCE_STATES& after) const
{
	const auto barrier = resource::barrier(before, after);

	command_list->ResourceBarrier(1, &barrier);
}
