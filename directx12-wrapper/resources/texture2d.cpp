#include "texture2d.hpp"

wrapper::directx12::clear_value::clear_value(float red, float green, float blue, float alpha) :
	red(red), green(green), blue(blue), alpha(alpha)
{
}

wrapper::directx12::clear_value::clear_value(float depth, uint8 stencil) :
	depth(depth), stencil(stencil)
{
}

wrapper::directx12::texture2d::texture2d(const ComPtr<ID3D12Resource>& source) : resource(source)
{
	assert(mDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D);
}

void wrapper::directx12::texture2d::copy_from(const graphics_command_list& command_list, const buffer& buffer) const
{
	D3D12_TEXTURE_COPY_LOCATION dest;
	D3D12_TEXTURE_COPY_LOCATION src;

	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;
	dest.pResource = mWrapperInstance.Get();

	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Format = mDesc.Format;
	src.PlacedFootprint.Footprint.Width = static_cast<UINT>(mDesc.Width);
	src.PlacedFootprint.Footprint.Height = static_cast<UINT>(mDesc.Height);
	src.PlacedFootprint.Footprint.Depth = 1;
	src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(alignment());
	src.pResource = buffer.get();

	D3D12_BOX region = {
		0, 0, 0,
		static_cast<UINT>(mDesc.Width),
		static_cast<UINT>(mDesc.Height),
		1
	};

	command_list->CopyTextureRegion(&dest, 0, 0, 0, &src, &region);
}

void wrapper::directx12::texture2d::copy_from(const graphics_command_list& command_list, const texture2d& texture) const
{
	assert(mDesc.Height == texture.mDesc.Height);
	assert(mDesc.Width == texture.mDesc.Width);

	D3D12_TEXTURE_COPY_LOCATION dest;
	D3D12_TEXTURE_COPY_LOCATION src;

	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;
	dest.pResource = mWrapperInstance.Get();

	src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	src.SubresourceIndex = 0;
	src.pResource = texture.get();

	D3D12_BOX region = {
		0, 0, 0,
		static_cast<UINT>(mDesc.Width),
		static_cast<UINT>(mDesc.Height),
		1
	};

	command_list->CopyTextureRegion(&dest, 0, 0, 0, &src, &region);
}

size_t wrapper::directx12::texture2d::alignment() const
{
	return align_to(mDesc.Width * size_of(mDesc.Format), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
}

DXGI_FORMAT wrapper::directx12::texture2d::format() const
{
	return mDesc.Format;
}

size_t wrapper::directx12::texture2d::width() const
{
	return static_cast<size_t>(mDesc.Width);
}

size_t wrapper::directx12::texture2d::height() const
{
	return static_cast<size_t>(mDesc.Height);
}

wrapper::directx12::texture2d wrapper::directx12::texture2d::create(
	const device& device, const resource_info& info,
	const DXGI_FORMAT& format, size_t width, size_t height, const clear_value& clear)
{
	return create(device, info, format, width, height, 1, clear);
}

wrapper::directx12::texture2d wrapper::directx12::texture2d::create(
	const device& device, const resource_info& info,
	const DXGI_FORMAT& format, size_t width, size_t height, size_t samples,
	const clear_value& clear)
{
	D3D12_RESOURCE_DESC desc;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = static_cast<UINT64>(width);
	desc.Height = static_cast<UINT>(height);
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = format;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = static_cast<UINT>(samples);
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = info.flags();

	D3D12_HEAP_PROPERTIES properties = {
		info.type(),
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	ComPtr<ID3D12Resource> texture;

	D3D12_CLEAR_VALUE value = {};
	auto need_clear_value = false;
	
	value.Format = desc.Format;

	//D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET can not be used with D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	if ((info.flags() & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0) {
		value.Color[0] = clear.red;
		value.Color[1] = clear.green;
		value.Color[2] = clear.blue;
		value.Color[3] = clear.alpha;

		need_clear_value = true;
	}

	if ((info.flags() & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0) {
		value.DepthStencil.Depth = clear.depth;
		value.DepthStencil.Stencil = clear.stencil;

		need_clear_value = true;
	}

	device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, info.state(), need_clear_value ? &value : nullptr, IID_PPV_ARGS(texture.GetAddressOf()));

	return texture2d(texture);
}
