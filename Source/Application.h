#pragma once

#include<list>
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

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() const { return render; }
    ModuleWindow* GetWindow() const { return window; }
    ModuleInput*  GetInput() const { return input; }
    ModuleProgram* GetProgram() const { return program; }
    ModuleDebugDraw* GetDebug() const { return debug; }
    ModuleEditorCamera* GetCamera() const { return camera; }
    ModuleEditor* GetEditor() const { return editor; }

    void RequestBrowser(const char* url) const;

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* exercise = nullptr;
    ModuleDebugDraw* debug = nullptr;
    ModuleEditorCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;

    std::list<Module*> modules;

};

extern Application* App;
