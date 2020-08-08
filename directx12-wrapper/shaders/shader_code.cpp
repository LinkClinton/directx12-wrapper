#include "shader_code.hpp"

#ifdef __ENABLE_DIRECTX_COMPILER__

#include <dxcapi.h>

#endif

wrapper::directx12::shader_code::shader_code(const std::vector<byte>& code) : mCode(code)
{
}

const byte* wrapper::directx12::shader_code::data() const
{
	return mCode.data();
}

size_t wrapper::directx12::shader_code::size() const
{
	return mCode.size();
}

wrapper::directx12::shader_code wrapper::directx12::shader_code::create(const std::vector<byte>& code)
{
	return shader_code(code);
}

#ifdef __ENABLE_DIRECTX_COMPILER__

wrapper::directx12::shader_code wrapper::directx12::shader_code::create(
	const std::wstring& filename, const std::wstring& entry, const std::wstring& version)
{
	ComPtr<IDxcIncludeHandler> include;
	ComPtr<IDxcCompiler> compiler;
	ComPtr<IDxcLibrary> library;

	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(library.GetAddressOf()));
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler.GetAddressOf()));

	library->CreateIncludeHandler(include.GetAddressOf());

	ComPtr<IDxcBlobEncoding> encoding_blob;

	library->CreateBlobFromFile(filename.c_str(), nullptr, encoding_blob.GetAddressOf());

	ComPtr<IDxcOperationResult> result;

	std::vector<DxcDefine> defines;

	defines.push_back({ L"__HLSL_SHADER__", L"1" });

	const wchar_t* arguments[] = {
		L"-Od",
		L"-Zi"
	};

#ifdef _DEBUG
	compiler->Compile(encoding_blob.Get(), filename.c_str(), entry.c_str(),
		version.c_str(), arguments, 1, defines.data(), static_cast<UINT32>(defines.size()),
		include.Get(), result.GetAddressOf());
#else
	compiler->Compile(encoding_blob.Get(), filename.c_str(), entry.c_str(),
		version.c_str(), nullptr, 0, defines.data(), static_cast<UINT32>(defines.size()),
		include.Get(), result.GetAddressOf());
#endif

	ComPtr<IDxcBlobEncoding> error_code;
	ComPtr<IDxcBlob> result_code;
	HRESULT result_status;

	result->GetErrorBuffer(error_code.GetAddressOf());
	result->GetResult(result_code.GetAddressOf());
	result->GetStatus(&result_status);

	std::string error;

	error.resize(error_code->GetBufferSize());

	std::memcpy(error.data(), error_code->GetBufferPointer(), error_code->GetBufferSize());

	if (!error.empty() && error.back() == '\n') error.pop_back();
	if (!error.empty()) printf("%s\n", error.c_str());

	auto code = std::vector<byte>(result_code->GetBufferSize());
	
	std::memcpy(code.data(), result_code->GetBufferPointer(), result_code->GetBufferSize());

	return shader_code(code);
}

#endif