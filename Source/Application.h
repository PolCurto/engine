#pragma once

#include <list>
#include <chrono>
#include "Globals.h"
#include "Module.h"

class Module;
class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleHardware;
class ModuleTextures;
class ModuleLightning;
class Model;

class Application
{
public:

	Application();
	~Application();

    bool PreInit();
	bool Init();
	update_status Update();
	bool CleanUp();

    void RequestBrowser(const char* url) const;

    ModuleOpenGL* GetOpenGL() const { return render; }
    ModuleWindow* GetWindow() const { return window; }
    ModuleInput*  GetInput() const { return input; }
    ModuleProgram* GetProgram() const { return program; }
    ModuleDebugDraw* GetDebug() const { return debug; }
    ModuleEditorCamera* GetCamera() const { return camera; }
    ModuleEditor* GetEditor() const { return editor; }
    ModuleTextures* GetTextures() const { return textures; }
    ModuleRenderExercise* GetRenderExercise() const { return exercise; }
    ModuleHardware* GetHardware() const { return hardware; }
    ModuleLightning* GetLightning() const { return lightning; }

public:
    float delta = 0;
    float fps = 0;

private:
    void CountFPS();

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* exercise = nullptr;
    ModuleDebugDraw* debug = nullptr;
    ModuleEditorCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleHardware* hardware = nullptr;
    ModuleTextures* textures = nullptr;
    ModuleLightning* lightning = nullptr;

    std::list<Module*> modules;

    std::chrono::steady_clock::time_point last_time;
    std::chrono::duration<float, std::milli> time_lapse;
    float frames = 0;
    float sec_timer = 0;
};

extern Application* App;
