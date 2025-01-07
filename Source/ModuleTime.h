#pragma once

#include "Module.h"
#include <memory>

class Timer;
class PreciseTimer;

class ModuleTime : public Module
{
public:

	ModuleTime();
	~ModuleTime();

	bool PreInit();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();


private:

	std::unique_ptr<Timer> timer;
	std::unique_ptr<PreciseTimer> precise_timer;

	unsigned int frame_count;
	float time_scale;
	float time;
	float real_time;
	float delta_time;
	float real_delta_time;

};
