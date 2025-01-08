#pragma once

#include "Timer.h"

class PreciseTimer : public Timer
{
public:
	PreciseTimer();
	~PreciseTimer();

	void Start() override;
	float Read() override;
	float Stop() override;
	float TicksSinceStartup() const override;


private:
	float timer = 0;
	bool is_enabled = false;

	float start_time = 0;
	float delta_time = 0;
	float elapsed_time = 0;
	float real_elapsed_time = 0;
	float time_scale = 1.0f;

	static float frequency;

};
