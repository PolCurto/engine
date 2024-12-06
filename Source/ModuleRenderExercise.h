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
	update_status Update();
	bool CleanUp();

private:
	unsigned int program_id;

	std::unique_ptr<Model> model;
};