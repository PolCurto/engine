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
#include "ModuleDebugDraw.h"
#include "ModuleWindow.h"
#include <Math/MathAll.h>	


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
	unsigned vtx_id = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, vtx_source);
	delete vtx_source;
	LOG("Vertex shader id: %d", vtx_id);

	// Compile the fragment shader
	char* frag_source = App->GetProgram()->LoadShaderSource("default_fragment.glsl");
	unsigned frag_id = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, frag_source);
	delete frag_source;
	LOG("Fragment shader id: %d", frag_id);

	// Create a program with Hello World vertex and fragment shaders
	program_id = App->GetProgram()->CreateProgram(vtx_id, frag_id);
	LOG("Program id: %d", program_id);

    CubemapSettings();

	// Load model and focus the camera
	model->Load("Models/BakerHouse/BakerHouse.gltf");
	App->GetCamera()->FocusGeometry();
	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
    // Render the skybox
    App->GetOpenGL()->SetDepthFunc(false);
    RenderSkybox();
    App->GetOpenGL()->SetDepthFunc(true);

    // Render the meshes
	model->Render(program_id);

    // Draw the grid
    float4x4 projection = App->GetCamera()->GetProjectionMatrix();
    float4x4 view = App->GetCamera()->GetViewMatrix();
    App->GetDebug()->Draw(view, projection, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());


	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	App->GetProgram()->DeleteProgram(program_id);
    App->GetProgram()->DeleteProgram(skybox_program_id);
	model->Delete();

	return true;
}

void ModuleRenderExercise::OnFileDropped(const char* filename) const
{
	model->Delete();
	model->Load(filename);
	App->GetCamera()->FocusGeometry();
}

void ModuleRenderExercise::CubemapSettings()
{
    // Skybox cubemap
    float skybox_vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenBuffers(1, &skybox_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);

    skybox_texture = App->GetTextures()->LoadCubemap("Textures/cubemap.dds");

    // We need new shaders to render the skybox
    char* skybox_vtx_source = App->GetProgram()->LoadShaderSource("skybox_vertex.glsl");
    unsigned skybox_vtx_id = App->GetProgram()->CompileShader(GL_VERTEX_SHADER, skybox_vtx_source);
    delete skybox_vtx_source;
    LOG("Skybox vertex shader id: %d", skybox_vtx_id);

    char* skybox_frag_source = App->GetProgram()->LoadShaderSource("skybox_fragment.glsl");
    unsigned skybox_frag_id = App->GetProgram()->CompileShader(GL_FRAGMENT_SHADER, skybox_frag_source);
    delete skybox_frag_source;
    LOG("Skybox fragment shader id: %d", skybox_frag_id);

    skybox_program_id = App->GetProgram()->CreateProgram(skybox_vtx_id, skybox_frag_id);
    LOG("Skybox program id: %d", skybox_program_id);
}

void ModuleRenderExercise::RenderSkybox() const
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();

	glUseProgram(skybox_program_id);
	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
}