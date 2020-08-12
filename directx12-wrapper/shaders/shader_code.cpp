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
	
	D3DCompile(source.data(), source.length(), nullptr, nullptr, nullptr, entry.c_str(), version.c_str(),
		0, 0, result.GetAddressOf(), message.GetAddressOf());

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