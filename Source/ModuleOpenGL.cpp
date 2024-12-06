#include "Globals.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleEditorCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Imgui/imgui.h"

void __stdcall OpenGlDebugging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

ModuleOpenGL::ModuleOpenGL()
{
	context = nullptr;
}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	context = SDL_GL_CreateContext(App->GetWindow()->window);

	GLenum err = glewInit();
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	// Information of the software and hardware
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Pipeline options
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	// Debugging
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(&OpenGlDebugging, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	// Get the window size
	glViewport(0, 0, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	glViewport(0, 0, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{
	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
	float4x4 view = App->GetCamera()->GetViewMatrix();
	// Draw debug axis origin and square grid
	App->GetDebug()->Draw(view, projection, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{

	LOG("Destroying renderer");

	SDL_GL_DeleteContext(context);

	//Destroy window

	return true;
}

//unsigned ModuleOpenGL::CreateTriangleVBO(float vertex_data[], int data_length)
//{
//	unsigned vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
//	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertex_data) * data_length, vertex_data, GL_STATIC_DRAW);
//
//	return vbo;
//}
//
//void ModuleOpenGL::RenderVBO(unsigned vbo, unsigned program, unsigned texture) const
//{
//	float4x4 projection = App->GetCamera()->GetProjectionMatrix();
//	float4x4 view = App->GetCamera()->GetViewMatrix();
//	float4x4 model = math::float4x4::FromTRS(float3(0.0f, 2.0f, -3.0f), float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));
//
//	glUseProgram(program);
//	glUniformMatrix4fv(0, 1, GL_TRUE, &projection[0][0]);
//	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
//	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);
//
//	// Bind buffer and vertex attributes
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//	// Bind uvs
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 12));     // The pointer is in location sizeof(float) * 3 * 12 because we have already drawn the triangles,
//																						     // which are three positions with x, y and z values.
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	
//	ImGui::Begin("Baboon Texture");
//	
//	ImGui::SeparatorText("Texture Data");
//	
//	//ImGui::Text("Texture width: %i", baboon_metadata.width);
//	//ImGui::Text("Texture height: %i", baboon_metadata.height);
//	//ImGui::Text("Format: %i", baboon_metadata.format);
//	//ImGui::Text("Mipmaps: %i", baboon_metadata.mipLevels);
//	
//	ImGui::SeparatorText("Texture settings");
//	
//	static int wrap_mode = GL_CLAMP;
//	if (ImGui::CollapsingHeader("Wrap mode"))
//	{
//		ImGui::RadioButton("Repeat##2", &wrap_mode, GL_REPEAT);
//		ImGui::SameLine();
//		ImGui::RadioButton("Mirrored repeat##2", &wrap_mode, GL_MIRRORED_REPEAT);
//		ImGui::RadioButton("Clamp##2", &wrap_mode, GL_CLAMP);
//		ImGui::SameLine();
//		ImGui::RadioButton("Clamp to border##2", &wrap_mode, GL_CLAMP_TO_BORDER);
//	}
//	
//	static int mag_filter = GL_NEAREST;
//	if (ImGui::CollapsingHeader("Mag Filter"))
//	{
//		ImGui::RadioButton("Nearest##1", &mag_filter, GL_NEAREST);
//		ImGui::SameLine();
//		ImGui::RadioButton("Linear##1", &mag_filter, GL_LINEAR);
//	}
//	
//	static int min_filter = GL_NEAREST;
//	if (ImGui::CollapsingHeader("Min Filter"))
//	{
//		ImGui::RadioButton("Nearest##2", &min_filter, GL_NEAREST);
//		ImGui::SameLine();
//		ImGui::RadioButton("Linear##2", &min_filter, GL_LINEAR);
//	}
//	
//	ImGui::End();
//	
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
//	
//	// 1 triangle to draw = 3 vertices
//	glDrawArrays(GL_TRIANGLES, 0, 12);
//}

void ModuleOpenGL::DestroyVBO(unsigned vbo) const
{
	glDeleteBuffers(1, &vbo);
}

void __stdcall OpenGlDebugging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}