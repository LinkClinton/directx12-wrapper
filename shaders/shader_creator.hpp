#pragma once

#include "shader_code.hpp"

namespace wrapper::directx12
{

	struct shader_variable
	{
		std::string semantic;
		std::string name;
		std::string type;
	};

	std::string to_string(const shader_variable& variable);
	
	class shader_creator final
	{
	public:
		shader_creator() = default;

		shader_creator(const std::string& source);

		shader_creator& include(const std::string& filepath, size_t line);

		shader_creator& include(const std::string& filepath);
		
		shader_creator& define_structure(const std::vector<shader_variable>& variables, const std::string& type_name);

		shader_creator& define_structured_buffer(const std::string& type, const std::string& name, uint32 base, uint32 space);
		
		shader_creator& define_constant_buffer(const std::string& type, const std::string& name, uint32 base, uint32 space);
		
		shader_creator& define_variable(const std::string& type, const std::string& name, size_t tab = 0);

		shader_creator& begin_function(const std::vector<shader_variable>& arguments, const std::string& type, const std::string& name);

		shader_creator& end_function();

		shader_creator& add_sentence(const std::string& sentence, size_t tab = 0);

		shader_creator& replace(const std::string& keyword, const shader_creator& creator);
		
		shader_creator& insert(const shader_creator& creator, size_t line = 0);

		std::string source() const noexcept;

		static shader_creator create_from_file(const std::wstring& filename);

		static shader_creator create();
	private:
		std::string prefix_tab(size_t count) const;
		
		std::vector<std::string> mSource;
	};
	
}
