#pragma once

#include "Module.h"
#include <memory>
#include "Timer.h"
#include "PreciseTimer.h"

class ModuleTime : public Module
{
public:

	ModuleTime();
	~ModuleTime();

	bool PreInit();
	bool Init();
	update_status Update();
	bool CleanUp();

	void CalcFps();
	void ShowTimers();
	void LimitFPS();

	void SetTimeScale(const float new_scale);

private:

	Timer timer;
	Timer game_clock;
	Timer realtime_clock;

	unsigned int frame_count = 0;
	float time_scale = 1.0f;
	float time = 0;
	float real_time = 0;
	float delta_time = 0;
	float real_delta_time = 0;

	unsigned int fps = 0;
	bool fps_is_limited = false;
	int fps_limit = 0;
	float desired_ms = 0;

	unsigned int fps_2 = 0;
	float fps_timer = 0;
	float fps_counter = 0;

};
