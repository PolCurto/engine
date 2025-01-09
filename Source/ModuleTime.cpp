#include "ModuleTime.h"
#include "Imgui/imgui.h"
#include "Globals.h"
#include "SDL.h"

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
	CalcFps();
	ShowTimers();

	if (fps_is_limited)
		LimitFPS();

	return UPDATE_CONTINUE;
}

bool ModuleTime::CleanUp()
{
	return true;
}

void ModuleTime::CalcFps()
{
	fps = 1000 / realtime_clock.GetDeltaTime();
	++frame_count;

	fps_timer += realtime_clock.GetDeltaTime();
	++fps_counter;
	if (fps_timer > 1000.0f)
	{
		fps_2 = fps_counter;
		fps_counter = fps_timer = 0;
	}
}

void ModuleTime::LimitFPS()
{
	desired_ms = 1000.0f / fps_limit;

	float frames_diff = desired_ms - realtime_clock.GetDeltaTime();
	if (frames_diff > 0)
	{
		SDL_Delay(frames_diff);
	}
}

void ModuleTime::ShowTimers()
{
	ImGui::Begin("Timers");

	ImGui::Text("Game clock: %.3f.", game_clock.Read() / 1000.0f);
	ImGui::SameLine();
	ImGui::Text("Delta time: %.2f", game_clock.GetDeltaTime());
	ImGui::Text("Real time clock: %.3f.", realtime_clock.Read() / 1000.0f);
	ImGui::SameLine();
	ImGui::Text("Real delta time: %.2f", realtime_clock.GetDeltaTime());

	if (ImGui::SliderFloat("Time scale", &time_scale, 0, 4))
		SetTimeScale(time_scale);

	ImGui::Separator();

	ImGui::Text("FPS 1: %d.", fps);
	ImGui::SameLine();
	ImGui::Text("FPS 2: %d.", fps_2);
	ImGui::SameLine();
	ImGui::Text("Frame count: %d", frame_count);

	ImGui::Checkbox("Limit FPS", &fps_is_limited);
	ImGui::SameLine();
	ImGui::SliderInt("Max FPS", &fps_limit, 10, 200);
	ImGui::Text("Desired ms: %f", desired_ms);

	ImGui::End();
}

void ModuleTime::SetTimeScale(const float new_scale)
{
	game_clock.SetTimeScale(new_scale);
}
