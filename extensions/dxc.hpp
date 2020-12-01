#pragma once

#define __DIRECTX_WRAPPER_EXTENSION_DXC__
#ifndef __DIRECTX_WRAPPER_EXTENSION_DXC__
#else

#include "../shaders/shader_creator.hpp"
#include "../shaders/shader_code.hpp"

#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

namespace wrapper::directx12::extensions {

	inline shader_code compile_from_source_using_dxc(const std::string& source, const std::wstring& filename, 
		const std::wstring& entry, const std::wstring& version)
	{
		ComPtr<IDxcIncludeHandler> include;
		ComPtr<IDxcCompiler> compiler;
		ComPtr<IDxcLibrary> library;
		
		DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(library.GetAddressOf()));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler.GetAddressOf()));

		library->CreateIncludeHandler(include.GetAddressOf());
		
		ComPtr<IDxcBlobEncoding> encoding_blob;
		
		library->CreateBlobWithEncodingFromPinned(source.c_str(), static_cast<uint32>(source.size()),
			CP_ACP, encoding_blob.GetAddressOf());
		
		ComPtr<IDxcOperationResult> result;

		std::vector<DxcDefine> defines;

		defines.push_back({ L"__HLSL_SHADER__", L"1" });

		std::vector<const wchar_t*> arguments = {
#ifdef _DEBUG
			L"-Od"
#endif
		};
		
		compiler->Compile(encoding_blob.Get(), filename.c_str(), entry.c_str(),
			version.c_str(), arguments.data(), static_cast<uint32>(arguments.size()), 
			defines.data(), static_cast<UINT32>(defines.size()),
			include.Get(), result.GetAddressOf());
		
		ComPtr<IDxcBlobEncoding> error_code;
		ComPtr<IDxcBlob> result_code;
		HRESULT result_status;

		result->GetErrorBuffer(error_code.GetAddressOf());
		result->GetResult(result_code.GetAddressOf());
		result->GetStatus(&result_status);

		auto error = copy_data_to_string(error_code->GetBufferPointer(), error_code->GetBufferSize());

		if (!error.empty() && error.back() == '\n') error.pop_back();
		if (!error.empty()) directx12::error(error);

		auto code = std::vector<byte>(result_code->GetBufferSize());

		std::memcpy(code.data(), result_code->GetBufferPointer(), result_code->GetBufferSize());

		return shader_code(code);
	}
	
	inline shader_code compile_from_file_using_dxc(const std::wstring& filename, const std::wstring& entry,
	                                               const std::wstring& version)
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

		std::vector<const wchar_t*> arguments = {
#ifdef _DEBUG
			L"-Od"
#endif
		};

		compiler->Compile(encoding_blob.Get(), filename.c_str(), entry.c_str(),
			version.c_str(), arguments.data(), static_cast<uint32>(arguments.size()),
			defines.data(), static_cast<UINT32>(defines.size()),
			include.Get(), result.GetAddressOf());

		ComPtr<IDxcBlobEncoding> error_code;
		ComPtr<IDxcBlob> result_code;
		HRESULT result_status;

		result->GetErrorBuffer(error_code.GetAddressOf());
		result->GetResult(result_code.GetAddressOf());
		result->GetStatus(&result_status);

		auto error = copy_data_to_string(error_code->GetBufferPointer(), error_code->GetBufferSize());
		
		if (!error.empty() && error.back() == '\n') error.pop_back();
		if (!error.empty()) directx12::error(error);

		auto code = std::vector<byte>(result_code->GetBufferSize());

		std::memcpy(code.data(), result_code->GetBufferPointer(), result_code->GetBufferSize());

		return shader_code(code);
	}

}

#endif