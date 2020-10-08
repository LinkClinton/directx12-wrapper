#pragma once

#include "shader_code.hpp"

namespace wrapper::directx12 {

	struct shader_variable {
		std::string semantic;
		std::string name;
		std::string type;
	};

	std::string to_string(const shader_variable& variable);
	
	class shader_creator {
	public:
		shader_creator() = default;

		~shader_creator() = default;

		shader_creator& define_structure(const std::vector<shader_variable>& variables, const std::string& type_name);

		shader_creator& define_structured_buffer(const std::string& type, const std::string& name, uint32 base, uint32 space);
		
		shader_creator& define_constant_buffer(const std::string& type, const std::string& name, uint32 base, uint32 space);
		
		shader_creator& define_variable(const std::string& type, const std::string& name);

		shader_creator& begin_function(const std::vector<shader_variable>& arguments, const std::string& type, const std::string& name);

		shader_creator& end_function();

		shader_creator& add_sentence(const std::string& sentence);
		
		std::string source() const noexcept;
	private:
		std::string mSource;
	};
	
}
