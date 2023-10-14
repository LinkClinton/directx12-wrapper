#pragma once

#include "shader_code.hpp"

namespace wrapper::directx12
{

	class shader_library final
	{
	public:
		shader_library() = default;
		
		shader_library(const shader_code& shader, const std::vector<std::wstring>& functions);

		shader_library(const shader_library& library);

		shader_library(shader_library&& library) noexcept;

		D3D12_DXIL_LIBRARY_DESC desc() const noexcept;

		shader_library& operator=(const shader_library& rhs);
		shader_library& operator=(shader_library&& rhs) noexcept;

		static shader_library create(const shader_code& shader, const std::vector<std::wstring>& functions);
	private:
		std::vector<D3D12_EXPORT_DESC> mExports;
		std::vector<std::wstring> mFunctions;

		D3D12_DXIL_LIBRARY_DESC mDesc = {};
		
		shader_code mShader;
	};
	
}
