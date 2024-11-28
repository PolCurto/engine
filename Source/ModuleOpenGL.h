#pragma once
#include "Module.h"
#include "DirectXTex.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned CreateTriangleVBO(float vertex_data[], int data_length);
	void RenderVBO(unsigned vbo, unsigned program, unsigned texture) const;
	void DestroyVBO(unsigned vbo) const;

	unsigned LoadTextureData(const DirectX::ScratchImage& image) const;

	void* GetContext() const { return context; }

	//void __stdcall OpenGlDebugging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context;
};
