#include "ModuleRenderExercise.h"
#include "ModuleOpenGL.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "DirectXTex.h"
#include "Model.h"
#include "Mesh.h"

ModuleRenderExercise::ModuleRenderExercise()
{
	program_id = 0;
	model = std::make_unique<Model>();
}

ModuleRenderExercise::~ModuleRenderExercise()
{

}

bool ModuleRenderExercise::Init()
{
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

	// Load model and focus the camera
	model->Load("Models/BakerHouse/BakerHouse.gltf");
	App->GetCamera()->FocusGeometry();
	return true;
}

update_status ModuleRenderExercise::Update()
{
	model->Render(program_id);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	App->GetProgram()->DeleteProgram(program_id);
	model->Delete();

	return true;
}

void ModuleRenderExercise::OnFileDropped(const char* filename) const
{
	model->Delete();
	model->Load(filename);
	App->GetCamera()->FocusGeometry();
}