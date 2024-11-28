#pragma once
#include "Module.h"
#include <vector>

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
	unsigned int texture;
	unsigned int program_id;
};