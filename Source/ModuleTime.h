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

	void ShowTimers();

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

	int fps_limit = 0;

};
