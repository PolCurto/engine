#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include <deque>
#include <vector>
#include <string>

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
	bool CleanUp();

	void AddLog(const char* new_log) const;
	void AddFPS(float new_fps);
	void ShowModelProperties() const;


private:
	void Draw();
	void MainMenu();

	void SettingsMenu();
	void WindowConfig() const;
	void CameraConfig() const;
	void OpenGLConfig() const;
	void TexturesConfig() const;
	void ModelConfig() const;
	void ShadingConfig() const;

	void FPSCount();
	void AboutWindow() const;
	void Console() const;

	bool show_demo = false;
	bool show_about = false;
	bool show_settings = false;
	bool show_properties = false;
	bool show_console = true;

	bool quit = false;

	int fps_counter = 0;
	std::deque<float> fps_log;
	std::deque<float> ms_log;

	ImGuiTextBuffer* logs;
};

#endif // __MODULE_EDITOR_H__
