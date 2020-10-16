#include "descriptor_heap.hpp"

wrapper::directx12::cpu_descriptor_handle::cpu_descriptor_handle(const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	ptr = handle.ptr;
}

wrapper::directx12::cpu_descriptor_handle& wrapper::directx12::cpu_descriptor_handle::operator+(const cpu_descriptor_handle& rhs)
{
	ptr += rhs.ptr; return *this;
}

wrapper::directx12::cpu_descriptor_handle& wrapper::directx12::cpu_descriptor_handle::operator-(const cpu_descriptor_handle& rhs)
{
	ptr -= rhs.ptr; return *this;
}

wrapper::directx12::cpu_descriptor_handle& wrapper::directx12::cpu_descriptor_handle::operator+(size_t offset)
{
	ptr += offset; return *this;
}

wrapper::directx12::cpu_descriptor_handle& wrapper::directx12::cpu_descriptor_handle::operator-(size_t offset)
{
	ptr -= offset; return *this;
}

wrapper::directx12::gpu_descriptor_handle::gpu_descriptor_handle(const D3D12_GPU_DESCRIPTOR_HANDLE& handle)
{
	ptr = handle.ptr;
}

wrapper::directx12::gpu_descriptor_handle& wrapper::directx12::gpu_descriptor_handle::operator+(const gpu_descriptor_handle& rhs)
{
	ptr += rhs.ptr; return *this;
}

wrapper::directx12::gpu_descriptor_handle& wrapper::directx12::gpu_descriptor_handle::operator-(const gpu_descriptor_handle& rhs)
{
	ptr -= rhs.ptr; return *this;
}

wrapper::directx12::gpu_descriptor_handle& wrapper::directx12::gpu_descriptor_handle::operator+(size_t offset)
{
	ptr += offset; return *this;
}

wrapper::directx12::gpu_descriptor_handle& wrapper::directx12::gpu_descriptor_handle::operator-(size_t offset)
{
	ptr -= offset; return *this;
}

wrapper::directx12::descriptor_table& wrapper::directx12::descriptor_table::add_srv_range(const std::vector<std::string>& name, size_t base, size_t space)
{
	return add_range(name, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, base, space);
}

wrapper::directx12::descriptor_table& wrapper::directx12::descriptor_table::add_uav_range(const std::vector<std::string>& name, size_t base, size_t space)
{
	return add_range(name, D3D12_DESCRIPTOR_RANGE_TYPE_UAV, base, space);
}

wrapper::directx12::descriptor_table& wrapper::directx12::descriptor_table::add_cbv_range(const std::vector<std::string>& name, size_t base, size_t space)
{
	return add_range(name, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, base, space);
}

wrapper::directx12::descriptor_table& wrapper::directx12::descriptor_table::add_range(
	const std::vector<std::string>& name, const D3D12_DESCRIPTOR_RANGE_TYPE& type, size_t base, size_t space)
{
	D3D12_DESCRIPTOR_RANGE range;

	range.OffsetInDescriptorsFromTableStart = static_cast<UINT>(mDescriptorsIndex.size());
	range.BaseShaderRegister = static_cast<UINT>(base);
	range.NumDescriptors = static_cast<UINT>(name.size());
	range.RegisterSpace = static_cast<UINT>(space);
	range.RangeType = type;

	for (size_t index = 0; index < name.size(); index++)
		mDescriptorsIndex.insert({ name[index], mDescriptorsIndex.size() });

	mRanges.push_back(range);

	return *this;
}

D3D12_ROOT_PARAMETER wrapper::directx12::descriptor_table::parameter() const
{
	D3D12_ROOT_PARAMETER parameter;

	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameter.DescriptorTable.pDescriptorRanges = mRanges.data();
	parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(mRanges.size());

	return parameter;
}

size_t wrapper::directx12::descriptor_table::index(const std::string& name) const
{
	return mDescriptorsIndex.at(name);
}

size_t wrapper::directx12::descriptor_table::count() const
{
	return mDescriptorsIndex.size();
}

wrapper::directx12::descriptor_heap::descriptor_heap(const ComPtr<ID3D12DescriptorHeap>& source, const ComPtr<ID3D12Device5>& device)
	: wrapper_t<ID3D12DescriptorHeap>(source)
{
	const auto desc = mWrapperInstance->GetDesc();

	mOffset = device->GetDescriptorHandleIncrementSize(desc.Type);
	mCount = static_cast<size_t>(desc.NumDescriptors);
}

D3D12_CPU_DESCRIPTOR_HANDLE wrapper::directx12::descriptor_heap::cpu_handle(size_t index) const
{
	assert(index < mCount);
	
	return offset_handle(mWrapperInstance->GetCPUDescriptorHandleForHeapStart(), index * mOffset);
}

D3D12_GPU_DESCRIPTOR_HANDLE wrapper::directx12::descriptor_heap::gpu_handle(size_t index) const
{
	assert(index < mCount);

	return offset_handle(mWrapperInstance->GetGPUDescriptorHandleForHeapStart(), index * mOffset);
}

wrapper::directx12::descriptor_heap wrapper::directx12::descriptor_heap::create(const device& device,
	const D3D12_DESCRIPTOR_HEAP_TYPE& type, size_t count)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;

	desc.NodeMask = 0;
	desc.Flags = type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = type;
	desc.NumDescriptors = static_cast<UINT>(count);

	ComPtr<ID3D12DescriptorHeap> heap;
	
	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heap.GetAddressOf()));

	return descriptor_heap(heap, device.get());
}

wrapper::directx12::descriptor_heap wrapper::directx12::descriptor_heap::create(const device& device, const descriptor_table& table)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;

	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = static_cast<UINT>(table.count());

	ComPtr<ID3D12DescriptorHeap> heap;

	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heap.GetAddressOf()));

	return descriptor_heap(heap, device.get());
}
