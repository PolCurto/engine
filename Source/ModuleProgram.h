#pragma once
#include "Module.h"

class ModuleProgram : public Module
{
public:

	ModuleProgram();
	~ModuleProgram();

	char* LoadShaderSource(const char* shader_file_name) const;
	unsigned int CompileShader(const unsigned int type, const char* source) const;
	unsigned int CreateProgram(const unsigned int vertex_shader, const unsigned int fragment_shader) const;
	void DeleteProgram(const unsigned int program_id) const;

private:

};