#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleEditorCamera.h"
#include "ModuleEditor.h"
#include "ModuleHardware.h"
#include "ModuleTextures.h"
#include "Model.h"
#include "Timer.h"
#include "PreciseTimer.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(camera = new ModuleEditorCamera());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(debug = new ModuleDebugDraw());
	modules.push_back(exercise = new ModuleRenderExercise());
	modules.push_back(hardware = new ModuleHardware());
	modules.push_back(editor = new ModuleEditor());

	timer = new Timer();
	precise_timer = new PreciseTimer();
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::PreInit()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->PreInit();

	return ret;
}

bool Application::Init()
{
	bool ret = true;


	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	timer->Start();
	precise_timer->Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	time_lapse = (std::chrono::steady_clock::now() - last_time);
	last_time = std::chrono::steady_clock::now();
	delta = time_lapse.count();
	CountFPS();

	//LOG("Delta: %f", GetDelta());

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	//LOG("Time: %d", timer->Read());
	timer->Read();
	precise_timer->Read();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	timer->Stop();
	precise_timer->Stop();
	//LOG("Elapsed time: %f\n", static_cast<float>(timer->Stop()) / 1000.0f);

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	delete timer;
	delete precise_timer;

	return ret;
}

void Application::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::CountFPS()
{
	if (sec_timer <= 1000.0f)
	{
		sec_timer += delta;
		++frames;
	}
	else
	{
		fps = frames;
		sec_timer = delta;
		editor->AddFPS(fps);
		frames = 0;
	}
	
}