#include "ModuleRenderExercise.h"
#include "ModuleOpenGL.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "DirectXTex.h"


ModuleRenderExercise::ModuleRenderExercise()
{
	program_id = 0;
}

ModuleRenderExercise::~ModuleRenderExercise()
{

}

bool ModuleRenderExercise::Init()
{
	float vtx_data[] = { 
						-1.0f, -1.0f, -3.0f,  // v0 pos
						1.0f, -1.0f, -3.0f,   // v1 pos
						-1.0f, 1.0f, -3.0f,   // v2 pos

						0.0f, 1.0f,           // v0 texcoord
						1.0f, 1.0f,           // v1 texcoord
						0.0f, 0.0f            // v2 texcoord
						};

	int data_length = sizeof(vtx_data) / sizeof(vtx_data[0]);

	// Load a triangle into a VBO
	vbos.emplace_back(App->GetOpenGL()->CreateTriangleVBO(vtx_data, data_length));

	float vtx_data2[] = { 
						1.0f, -1.0f, -3.0f,  // v0 pos 
						1.0f, 1.0f, -3.0f,   // v1 pos
						-1.0f, 1.0f, -3.0f,  // v2 pos

						1.0f, 1.0f,          // v0 texcoord
						1.0f, 0.0f,          // v1 texcoord
						0.0f, 0.0f           // v2 texcoord
						};

	data_length = sizeof(vtx_data2) / sizeof(vtx_data2[0]);
	vbos.emplace_back(App->GetOpenGL()->CreateTriangleVBO(vtx_data2, data_length));

	// Compile the vertex shader
	char* vtx_source = App->GetProgram()->LoadShaderSource("default_vertex.glsl");
	//LOG("Vertex source: %s", vtx_source);
	unsigned vtx_id = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vtx_source);
	free(vtx_source);
	LOG("Vertex shader id: %d", vtx_id);

	// Compile the fragment shader
	char* frag_source = App->GetProgram()->LoadShaderSource("default_fragment.glsl");
	//LOG("Fragment source: %s", frag_source);
	unsigned frag_id = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, frag_source);
	free(frag_source);
	LOG("Fragment shader id: %d", frag_id);

	// Create a program with Hello World vertex and fragment shaders
	program_id = App->GetProgram()->CreateProgram(vtx_id, frag_id);
	LOG("Program id: %d", program_id);

	DirectX::ScratchImage image;

	if (App->GetTextures()->LoadFile(L"Textures/Baboon.dds", image))
	{
		texture = App->GetOpenGL()->LoadTextureData(image);
	}
	else
	{
		LOG("Couldn't load texture");
	}

	return true;
}

update_status ModuleRenderExercise::Update()
{
	for (const unsigned int& vbo : vbos)
	{
		App->GetOpenGL()->RenderVBO(vbo, program_id, texture);
	}

	const uint8_t* keys = App->GetInput()->GetKeyboard();

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	for (const unsigned int& vbo : vbos)
	{
		App->GetOpenGL()->DestroyVBO(vbo);
	}
	App->GetProgram()->DeleteProgram(program_id);

	return true;
}