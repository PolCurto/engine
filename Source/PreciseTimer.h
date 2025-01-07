#pragma once

#include "Timer.h"

class PreciseTimer : public Timer
{
public:
	PreciseTimer();
	~PreciseTimer();

	void Start() override;
	unsigned int Read() override;
	unsigned int Stop() override;


private:
	float timer = 0;
	bool is_enabled = false;

	float start_time = 0;
	float elapsed_time = 0;

	static float frequency;

};
