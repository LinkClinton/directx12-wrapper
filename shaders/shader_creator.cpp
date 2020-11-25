#include "shader_creator.hpp"

#include <fstream>

std::string wrapper::directx12::to_string(const shader_variable& variable)
{
	return variable.type + " " + variable.name +
		(variable.semantic.empty() ? "" : ": " + variable.semantic);
}

wrapper::directx12::shader_creator::shader_creator(const std::string& source)
{
	std::string current_line;

	for (const auto& character : source) {
		current_line.push_back(character);
		
		if (character == '\n') {
			mSource.push_back(current_line);

			current_line.clear();
		}
	}
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::include(const std::string& filepath,
	uint32 line)
{
	mSource.insert(mSource.begin() + line, "#include \"" + filepath + "\"");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_structure(
	const std::vector<shader_variable>& variables, const std::string& type_name)
{
	mSource.push_back("struct " + type_name + "{\n");
	
	for (const auto& variable : variables) mSource.push_back("\t" + to_string(variable) + ";\n");

	mSource.push_back("};\n");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_structured_buffer(
	const std::string& type, const std::string& name, uint32 base, uint32 space)
{
	mSource.push_back("StructuredBuffer<" + type + "> " + name +
		" : register(t" + std::to_string(base) + ", space" + std::to_string(space) + ");\n");
	
	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_constant_buffer(
	const std::string& type, const std::string& name, uint32 base, uint32 space)
{
	mSource.push_back("ConstantBuffer<" + type + "> " + name + 
		" : register(b" + std::to_string(base) + ", space" + std::to_string(space) + ");\n");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_variable(const std::string& type, const std::string& name)
{
	mSource.push_back(type + " " + name + ";\n");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::begin_function(const std::vector<shader_variable>& arguments,
	const std::string& type, const std::string& name)
{
	mSource.push_back(type + " " + name + "(\n");

	for (size_t index = 0; index < arguments.size(); index++) {
		mSource.push_back("\t" + to_string(arguments[index]));

		if (index + 1 != arguments.size()) mSource.push_back(",\n");
	}

	mSource.push_back(") {\n");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::end_function()
{
	mSource.push_back("}\n");

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::add_sentence(const std::string& sentence)
{
	mSource.push_back(sentence + "\n");
	
	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::insert(const shader_creator& creator,
	uint32 line)
{
	mSource.insert(mSource.begin(), creator.mSource.begin(), creator.mSource.end());

	return *this;
}

std::string wrapper::directx12::shader_creator::source() const noexcept
{
	std::string source;

	for (const auto& line : mSource) source += line;

	return source;
}

wrapper::directx12::shader_creator wrapper::directx12::shader_creator::create_from_file(const std::wstring& filename)
{
	std::ifstream stream(filename);
	std::string source;
	std::string line;

	while (std::getline(stream, line))
		source += line + "\n";

	return shader_creator(source);
}
