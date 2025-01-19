#pragma once
#include "Module.h"
#include <memory>
#include <vector>

class Model;

class ModuleRenderExercise : public Module
{
public:

	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnFileDropped(const char* filename) const;
	void CubemapSettings();
	void RenderSkybox() const;

	std::unique_ptr<Model> model;

private:
	unsigned int program_id;
	unsigned int skybox_program_id;

	unsigned int skybox_vbo;
	unsigned int skybox_texture;

};