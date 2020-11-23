#include "buffer.hpp"

#include "texture2d.hpp"

#include <memory>

wrapper::directx12::buffer::buffer(const ComPtr<ID3D12Resource>& source) : resource(source)
{
	assert(mDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER);
}

void wrapper::directx12::buffer::copy_from_cpu(const void* data, size_t size) const
{
	assert(mHeapProperties.Type == D3D12_HEAP_TYPE_UPLOAD);

	byte* buffer_data = nullptr;

	mWrapperInstance->Map(0, nullptr, reinterpret_cast<void**>(&buffer_data));

	std::memcpy(buffer_data, data, size);

	mWrapperInstance->Unmap(0, nullptr);
}

void wrapper::directx12::buffer::copy_from(const graphics_command_list& command_list, const buffer& buffer) const
{
	command_list->CopyBufferRegion(mWrapperInstance.Get(), 0, buffer.get(), 0,
		static_cast<UINT64>(buffer.size_in_bytes()));
}

void wrapper::directx12::buffer::copy_from(const graphics_command_list& command_list, const texture2d& texture) const
{
	D3D12_TEXTURE_COPY_LOCATION dest;
	D3D12_TEXTURE_COPY_LOCATION src;

	dest.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	dest.PlacedFootprint.Offset = 0;
	dest.PlacedFootprint.Footprint.Format = texture.format();
	dest.PlacedFootprint.Footprint.Width = static_cast<UINT>(texture.size_x());
	dest.PlacedFootprint.Footprint.Height = static_cast<UINT>(texture.size_y());
	dest.PlacedFootprint.Footprint.Depth = 1;
	dest.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(texture.alignment());
	dest.pResource = mWrapperInstance.Get();

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.SubresourceIndex = 0;
	src.pResource = texture.get();

	D3D12_BOX region = {
		0, 0, 0,
		static_cast<UINT>(texture.size_x()),
		static_cast<UINT>(texture.size_y()),
		1
	};

	command_list->CopyTextureRegion(&dest, 0, 0, 0, &src, &region);
}

auto wrapper::directx12::buffer::begin_mapping() const -> void* 
{
	void* data = nullptr;

	mWrapperInstance->Map(0, nullptr, &data);

	return data;
}

void wrapper::directx12::buffer::end_mapping() const
{
	mWrapperInstance->Unmap(0, nullptr);
}

size_t wrapper::directx12::buffer::size_in_bytes() const
{
	return static_cast<size_t>(mDesc.Width);
}

wrapper::directx12::buffer wrapper::directx12::buffer::create(const device& device, const resource_info& info, size_t size)
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
	desc.Flags = info.flags();

	D3D12_HEAP_PROPERTIES properties = {
		info.type(),
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	ComPtr<ID3D12Resource> buffer;

	device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, info.state(), nullptr, IID_PPV_ARGS(buffer.GetAddressOf()));

	return directx12::buffer(buffer);
}
