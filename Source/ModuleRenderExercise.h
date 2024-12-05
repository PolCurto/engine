#pragma once
#include "Module.h"
#include <memory>
#include <vector>

class Mesh;

class ModuleRenderExercise : public Module
{
public:

	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	unsigned int vbo;
	int texture;
	unsigned int program_id;

	std::vector<std::unique_ptr<Mesh>> meshes;
};