#pragma once
#include "Module.h"

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

	void DestroyVBO(unsigned vbo) const;

	void* GetContext() const { return context; }

	void SetDepthTest(bool enable) const;
	void SetFaceCull(bool enable) const;
	void SetFrontFaceMode(int mode) const;
	void SetDepthFunc(bool enable) const;

	//void __stdcall OpenGlDebugging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	void* context;

};
