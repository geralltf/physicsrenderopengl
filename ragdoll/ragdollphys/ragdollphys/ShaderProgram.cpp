#include "ShaderProgram.h"

std::map<const char*, ShaderProgram*>* ShaderProgram::map_all_shaders;

void ShaderProgram::shader_hot_reload(std::string* file_name, SHADER_TYPE shader_type)
{
	if ((*file_name)[0] == '.' && (*file_name)[1] == '/') {
		*file_name = file_name->substr(2, file_name->size());
	}

	ShaderProgram* shader_program = nullptr;
	//ShaderProgram* shader_program = (*map_all_shaders)[file_name->c_str()];
	//auto m = map_all_shaders->emplace(file_name);
	std::string* eq;
	for (auto it = map_all_shaders->begin(); it != map_all_shaders->end(); ++it)
	{
		eq = new std::string(it->first);
		//std::cout << it->first << '\t' << it->second << std::endl;
		if (*eq == *file_name && it->second != nullptr)
		{
			shader_program = it->second;

			break;
		}

	}

	glDeleteProgram(shader_program->shaderProgram);

	shader_program->shaderProgram = glCreateProgram();

	shader_program->LoadShaderProgram(shader_program->_vertex_file_name, SHADER_TYPE::VertexShader, true);
	shader_program->LoadShaderProgram(shader_program->_frag_file_name, SHADER_TYPE::FragmentShader, true);

	shader_program->load(shader_program->_vertex_file_name, shader_program->_frag_file_name, true);

	shader_program->deinit();
	shader_program->init();
}