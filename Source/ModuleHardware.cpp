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
	ImGui::Text("SDL Version: %u.%u.%u", ver.major, ver.minor, ver.patch);

	ImGui::Separator();

	ImGui::Text("CPUs: %i (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

	float ram = static_cast<float>(SDL_GetSystemRAM()) / 1000;
	ImGui::Text("System RAM: %.2fGB", ram);
	ImGui::Text("Caps: ");

	ImGui::Separator();

	ImGui::Text("GPU: ");

	ImGui::End();


	return UPDATE_CONTINUE;
}