#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include <deque>

struct ImGuiTextBuffer;

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool PreInit();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void AddLog(const char* new_log) const;
	void AddFPS(float new_fps);

private:
	void Draw();
	void MainMenu();
	void SettingsWindow();
	void FPSCount();
	void AboutWindow() const;
	void Console() const;

	bool show_demo = false;
	bool show_about = false;
	bool show_settings = false;
	bool fullscreen = false;
	bool resizable = true;

	int fps_counter = 0;
	std::deque<float> fps_log;
	std::deque<float> ms_log;

	ImGuiTextBuffer* logs;
};

#endif // __MODULE_EDITOR_H__
