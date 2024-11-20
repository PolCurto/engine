#include "ModuleHardware.h"

#include "SDL.h"
#include "GL/glew.h"
#include "Imgui/imgui.h"

ModuleHardware::ModuleHardware()
{
}

ModuleHardware::~ModuleHardware()
{
}

update_status ModuleHardware::Update()
{
	ImGui::Begin("Hardware");

	SDL_version ver;
	SDL_GetVersion(&ver);
	ImGui::Text("SDL Version: %u", ver.major);

	ImGui::End();


	return UPDATE_CONTINUE;
}