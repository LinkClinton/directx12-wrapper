#include "shader_code.hpp"

#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler") 

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

wrapper::directx12::shader_code wrapper::directx12::shader_code::create_from_source(
	const std::string& source, const std::string& entry, const std::string& version)
{
	ComPtr<ID3DBlob> message;
	ComPtr<ID3DBlob> result;

	D3D_SHADER_MACRO macros[] = {
		{ "__HLSL_SHADER__", "1" },
		{ nullptr, nullptr }
	};

#ifdef _DEBUG
	const auto flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	const auto flag = 0;
#endif
	
	D3DCompile(source.data(), source.length(), nullptr, macros, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), version.c_str(),
		flag, 0, result.GetAddressOf(), message.GetAddressOf());

	std::string error;

	if (message != nullptr) {

		error.resize(message->GetBufferSize());

		std::memcpy(error.data(), message->GetBufferPointer(), message->GetBufferSize());

		if (!error.empty() && error.back() == '\n') error.pop_back();
		if (!error.empty()) printf("%s\n", error.c_str());
		
	}
		
	auto code = std::vector<byte>(result->GetBufferSize());

	std::memcpy(code.data(), result->GetBufferPointer(), result->GetBufferSize());

	return shader_code(code);
}

wrapper::directx12::shader_code wrapper::directx12::shader_code::create_from_file(
	const std::wstring& filename, const std::string& entry, const std::string& version,
	const std::vector<std::pair<std::string, std::string>>& macros)
{	
	ComPtr<ID3DBlob> message;
	ComPtr<ID3DBlob> result;

	std::vector<D3D_SHADER_MACRO> shader_macros = {
		{ "__HLSL_SHADER__", "1" },
		{ "__FXC_COMPILER__", "1" }
	};

	for (const auto& [name, value] : macros)
		shader_macros.push_back({ name.c_str(), value.c_str() });

	shader_macros.push_back({ nullptr, nullptr });

#ifdef _DEBUG
	const auto flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;;
#else
	const auto flag = 0;
#endif
	
	D3DCompileFromFile(filename.c_str(), shader_macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry.c_str(), version.c_str(), flag, 0, 
		result.GetAddressOf(), message.GetAddressOf());
	
	std::string error;

	if (message != nullptr) {

		error.resize(message->GetBufferSize());

		std::memcpy(error.data(), message->GetBufferPointer(), message->GetBufferSize());

		if (!error.empty() && error.back() == '\n') error.pop_back();
		if (!error.empty()) printf("%s\n", error.c_str());

	}

	auto code = std::vector<byte>(result->GetBufferSize());

	std::memcpy(code.data(), result->GetBufferPointer(), result->GetBufferSize());

	return shader_code(code);
}

wrapper::directx12::shader_code wrapper::directx12::shader_code::create_from_file(
	const std::string& filename, const std::string& entry, const std::string& version,
	const std::vector<std::pair<std::string, std::string>>& macros)
{
	return create_from_file(multi_bytes_string_to_wide_string(filename),
		entry, version, macros);
}
