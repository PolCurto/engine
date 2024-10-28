#include "ModuleRenderExercise.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "GL/glew.h"


ModuleRenderExercise::ModuleRenderExercise()
{
	vbo = 0;
	program_id = 0;
}

ModuleRenderExercise::~ModuleRenderExercise()
{

}

bool ModuleRenderExercise::Init()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	int data_length = sizeof(vtx_data) / sizeof(vtx_data[0]);

	// Load a triangle into a VBO
	vbo = App->GetOpenGL()->CreateTriangleVBO(vtx_data, data_length);
	LOG("VBO: %d", vbo);

	// Compile the vertex shader
	char* vtx_source = App->GetProgram()->LoadShaderSource("default_vertex.glsl");
	//LOG("Vertex source: %s", vtx_source);
	unsigned vtx_id = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vtx_source);
	LOG("Vertex shader id: %d", vtx_id);

	// Compile the fragment shader
	char* frag_source = App->GetProgram()->LoadShaderSource("default_fragment.glsl");
	//LOG("Fragment source: %s", frag_source);
	unsigned frag_id = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, frag_source);
	
	LOG("Fragment shader id: %d", frag_id);

	// Create a program with Hello World vertex and fragment shaders
	program_id = App->GetProgram()->CreateProgram(vtx_id, frag_id);
	LOG("Program id: %d", program_id);

	free(vtx_source);
	free(frag_source);

	return true;
}

update_status ModuleRenderExercise::Update()
{
	App->GetOpenGL()->RenderVBO(vbo, program_id);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	App->GetOpenGL()->DestroyVBO(vbo);
	App->GetProgram()->DeleteProgram(program_id);

	return true;
}