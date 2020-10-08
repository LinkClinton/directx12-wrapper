#include "shader_creator.hpp"

std::string wrapper::directx12::to_string(const shader_variable& variable)
{
	return variable.type + " " + variable.name +
		(variable.semantic.empty() ? "" : ": " + variable.semantic);
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_structure(
	const std::vector<shader_variable>& variables, const std::string& type_name)
{
	mSource += "struct " + type_name + " {\n";

	for (const auto& variable : variables)  mSource += "\t" + to_string(variable) + ";\n";

	mSource += "};\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_structured_buffer(
	const std::string& type, const std::string& name, uint32 base, uint32 space)
{
	mSource += "StructuredBuffer<" + type + "> " + name +
		" : register(t" + std::to_string(base) + ", space" + std::to_string(space) + ");\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_constant_buffer(
	const std::string& type, const std::string& name, uint32 base, uint32 space)
{
	mSource += "ConstantBuffer<" + type + "> " + name + 
		" : register(b" + std::to_string(base) + ", space" + std::to_string(space) + ");\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::define_variable(const std::string& type, const std::string& name)
{
	mSource += type + " " + name + ";\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::begin_function(const std::vector<shader_variable>& arguments,
	const std::string& type, const std::string& name)
{
	mSource += type + " " + name + "(\n";

	for (size_t index = 0; index < arguments.size(); index++) {
		mSource += "\t" + to_string(arguments[index]);

		if (index + 1 != arguments.size()) mSource += ",\n";
	}

	mSource += ") {\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::end_function()
{
	mSource += "}\n";

	return *this;
}

wrapper::directx12::shader_creator& wrapper::directx12::shader_creator::add_sentence(const std::string& sentence)
{
	mSource += sentence + "\n";
	
	return *this;
}

std::string wrapper::directx12::shader_creator::source() const noexcept
{
	return mSource;
}
