#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	void Draw();

	bool show_demo = false;
	bool fullscreen = false;
	bool resizable = false;
};

#endif // __MODULE_EDITOR_H__
