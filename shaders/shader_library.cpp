#include "shader_library.hpp"

wrapper::directx12::shader_library::shader_library(const shader_code& shader, const std::vector<std::wstring>& functions) :
	mFunctions(functions), mShader(shader)
{
	mExports = std::vector<D3D12_EXPORT_DESC>(mFunctions.size());

	for (size_t index = 0; index < mFunctions.size(); index++) {
		mExports[index].Flags = D3D12_EXPORT_FLAG_NONE;
		mExports[index].ExportToRename = nullptr;
		mExports[index].Name = mFunctions[index].c_str();
	}

	mDesc.NumExports = static_cast<UINT>(mExports.size());
	mDesc.pExports = mExports.data();
	mDesc.DXILLibrary.pShaderBytecode = mShader.data();
	mDesc.DXILLibrary.BytecodeLength = mShader.size();
}

wrapper::directx12::shader_library::shader_library(const shader_library& library) : shader_library(library.mShader, library.mFunctions)
{
}

wrapper::directx12::shader_library::shader_library(shader_library&& library) noexcept
{
	mFunctions = std::move(library.mFunctions);
	mExports = std::move(library.mExports);
	mShader = std::move(library.mShader);

	mDesc = library.mDesc;
}

D3D12_DXIL_LIBRARY_DESC wrapper::directx12::shader_library::desc() const noexcept
{
	return mDesc;
}

wrapper::directx12::shader_library& wrapper::directx12::shader_library::operator=(const shader_library& rhs)
{
	return *this = shader_library(rhs);
}

wrapper::directx12::shader_library& wrapper::directx12::shader_library::operator=(shader_library&& rhs) noexcept
{
	mFunctions = std::move(rhs.mFunctions);
	mExports = std::move(rhs.mExports);
	mShader = std::move(rhs.mShader);

	mDesc = rhs.mDesc;

	return *this;
}

wrapper::directx12::shader_library wrapper::directx12::shader_library::create(const shader_code& shader,
	const std::vector<std::wstring>& functions)
{
	return shader_library(shader, functions);
}

