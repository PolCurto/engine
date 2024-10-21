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

}

update_status ModuleProgram::Update()
{

}

bool ModuleProgram::CleanUp()
{

}

char* LoadShaderSource(const char* shader_file_name)
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

unsigned CompileShader(unsigned type, const char* source)
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
		}
	}
}