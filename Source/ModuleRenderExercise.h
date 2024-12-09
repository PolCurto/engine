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
	void OnFileDropped(const char* filename);

private:

	unsigned int program_id;
};