#include "ModuleTime.h"
#include "Imgui/imgui.h"
#include "Globals.h"

ModuleTime::ModuleTime()
{
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::PreInit()
{
	return true;
}

bool ModuleTime::Init()
{
	game_clock.Start();
	realtime_clock.Start();
	return true;
}

update_status ModuleTime::Update()
{
	ShowTimers();
	return UPDATE_CONTINUE;
}

bool ModuleTime::CleanUp()
{
	return true;
}

void ModuleTime::ShowTimers()
{
	ImGui::Begin("Timers");

	ImGui::Text("Game clock: %.3f", game_clock.Read() / 1000.0f);
	ImGui::SameLine();
	ImGui::Text("Delta time: %.2f", game_clock.GetDeltaTime());
	ImGui::Text("Real time clock: %.3f", realtime_clock.Read() / 1000.0f);
	ImGui::SameLine();
	ImGui::Text("Real delta time: %.2f", realtime_clock.GetDeltaTime());

	if (ImGui::SliderFloat("Time scale", &time_scale, 0, 4))
		SetTimeScale(time_scale);

	ImGui::End();
}

void ModuleTime::SetTimeScale(const float new_scale)
{
	game_clock.SetTimeScale(new_scale);
}
