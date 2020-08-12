#include "resource.hpp"

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
