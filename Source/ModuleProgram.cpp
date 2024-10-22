#include "ModuleProgram.h"
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{

}

ModuleProgram::~ModuleProgram()
{

}

bool ModuleProgram::Init()
{
	return true;
}

update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
	return true;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
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

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
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

unsigned ModuleProgram::CreateProgram(unsigned vertex_shader, unsigned fragment_shader)
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

void ModuleProgram::DeleteProgram(unsigned int program_id)
{
	glDeleteProgram(program_id);
}