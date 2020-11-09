#include "accleration.hpp"

wrapper::directx12::buffer wrapper::directx12::raytracing_geometry::acceleration() const noexcept
{
	return mAcceleration;
}

wrapper::directx12::raytracing_geometry wrapper::directx12::raytracing_geometry::create(
	const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS& flags, 
	const graphics_command_list& command_list, const device& device, 
	const gpu_virtual_address& vtx_address, uint32 vtx_count,
	const gpu_virtual_address& idx_address, uint32 idx_count)
{
	raytracing_geometry geometry;
	
	D3D12_RAYTRACING_GEOMETRY_DESC desc = {};

	desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	desc.Triangles.Transform3x4 = 0;
	desc.Triangles.VertexBuffer.StartAddress = vtx_address;
	desc.Triangles.VertexBuffer.StrideInBytes = 12;
	desc.Triangles.VertexCount = vtx_count;
	desc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.Triangles.IndexBuffer = idx_address;
	desc.Triangles.IndexCount = idx_count;
	desc.Triangles.IndexFormat = DXGI_FORMAT_R32_UINT;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};

	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = 1;
	inputs.Flags = flags;
	inputs.pGeometryDescs = &desc;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO build_info = {};

	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &build_info);

	build_info.ResultDataMaxSizeInBytes = align_to(build_info.ResultDataMaxSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);
	build_info.ScratchDataSizeInBytes = align_to(build_info.ScratchDataSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);

	geometry.mAcceleration = buffer::create(device, resource_info::common(
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		build_info.ResultDataMaxSizeInBytes);

	geometry.mScratch = buffer::create(device, resource_info::common(
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		build_info.ScratchDataSizeInBytes);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC acceleration_desc;

	acceleration_desc.Inputs = inputs;
	acceleration_desc.DestAccelerationStructureData = geometry.mAcceleration->GetGPUVirtualAddress();
	acceleration_desc.ScratchAccelerationStructureData = geometry.mAcceleration->GetGPUVirtualAddress();

	D3D12_RESOURCE_BARRIER barrier;

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = geometry.mAcceleration.get();
	
	command_list->BuildRaytracingAccelerationStructure(&acceleration_desc, 0, nullptr);
	command_list->ResourceBarrier(1, &barrier);

	return geometry;
}

wrapper::directx12::buffer wrapper::directx12::raytracing_acceleration::acceleration() const noexcept
{
	return mAcceleration;
}