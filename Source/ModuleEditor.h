#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include <deque>

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
	void MainMenu();
	void FPSCount();

	bool show_demo = false;
	bool fullscreen = false;
	bool resizable = true;

	int fps_counter = 0;
	std::deque<float> fps_log;
	std::deque<float> ms_log;
};

#endif // __MODULE_EDITOR_H__
