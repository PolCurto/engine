#include "ModuleProgram.h"
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{

}

ModuleProgram::~ModuleProgram()
{

}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name) const
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;    //Must free data after using it
}

unsigned int ModuleProgram::CompileShader(const unsigned int type, const char* source) const
{
	// Types GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);

	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		// If there is an error, gets the log
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Shader Log info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned int ModuleProgram::CreateProgram(const unsigned int vertex_shader, const unsigned int fragment_shader) const
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	int res = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}

	return program_id;
}

void ModuleProgram::DeleteProgram(const unsigned int program_id) const
{
	glDeleteProgram(program_id);
}