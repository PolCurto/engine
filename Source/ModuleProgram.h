#pragma once
#include "Module.h"

class ModuleProgram : public Module
{
public:

	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vertex_shader, unsigned fragment_shader);
	void DeleteProgram(unsigned program_id);

private:

};