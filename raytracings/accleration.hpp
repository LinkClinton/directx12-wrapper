#pragma once

#include "../resources/buffer.hpp"

namespace wrapper::directx12
{
	
	class raytracing_geometry final
	{
	public:
		raytracing_geometry() = default;

		explicit raytracing_geometry(const buffer& acceleration);

		buffer acceleration() const noexcept;
		
		static raytracing_geometry create(
			const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS& flags,
			const graphics_command_list& command_list, const device& device,
			const gpu_virtual_address& vtx_address, uint32 vtx_count,
			const gpu_virtual_address& idx_address, uint32 idx_count);
	private:
		buffer mAcceleration;
		buffer mScratch;
	};

	struct raytracing_instance
	{
		raytracing_geometry geometry;

		real transform[3][4];
	};

	class raytracing_acceleration final
	{
	public:
		raytracing_acceleration() = default;

		explicit raytracing_acceleration(const buffer& acceleration);

		buffer acceleration() const noexcept;

		static raytracing_acceleration create(
			const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS& flags,
			const graphics_command_list& command_list, const device& device,
			const std::vector<raytracing_instance>& instances);
	private:
		buffer mAcceleration;
		buffer mGpuInstances;
		buffer mCpuInstances;
		buffer mScratch;
	};
}