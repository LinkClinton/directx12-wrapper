#include "resource.hpp"

#include <memory>

wrapper::directx12::resource::resource(const ComPtr<ID3D12Resource>& source) : wrapper_t<ID3D12Resource>(source)
{
	mDesc = mWrapperInstance->GetDesc();

	mWrapperInstance->GetHeapProperties(&mHeapProperties, &mHeapFlags);
}

void wrapper::directx12::resource::copy_data_from_cpu(void* data, size_t size) const
{
	assert(mDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER);
	assert(mHeapProperties.Type == D3D12_HEAP_TYPE_UPLOAD);

	byte* buffer_data = nullptr;
	
	mWrapperInstance->Map(0, nullptr, reinterpret_cast<void**>(&buffer_data));

	std::memcpy(buffer_data, data, size);
	
	mWrapperInstance->Unmap(0, nullptr);
}

D3D12_RESOURCE_BARRIER wrapper::directx12::resource::barrier(const D3D12_RESOURCE_STATES& before, const D3D12_RESOURCE_STATES& after) const
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

void wrapper::directx12::resource::barrier(const graphics_command_list& command_list, const D3D12_RESOURCE_STATES& before, const D3D12_RESOURCE_STATES& after) const
{
	const auto barrier = resource::barrier(before, after);
	
	command_list->ResourceBarrier(1, &barrier);
}

wrapper::directx12::resource wrapper::directx12::resource::create(
	const device& device, const D3D12_RESOURCE_STATES& state, const D3D12_RESOURCE_FLAGS& flags, const D3D12_HEAP_TYPE& type, size_t size)
{
	D3D12_RESOURCE_DESC desc;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = static_cast<UINT64>(size);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = flags;

	D3D12_HEAP_PROPERTIES properties = {
		type,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	ComPtr<ID3D12Resource> buffer;

	device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, state, nullptr, IID_PPV_ARGS(buffer.GetAddressOf()));

	return resource(buffer);
}
