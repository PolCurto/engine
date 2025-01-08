#include "Globals.h"
#include "Application.h"
#include "PreciseTimer.h"
#include "SDL.h"

float PreciseTimer::frequency = 0.0f;

PreciseTimer::PreciseTimer()
{
	frequency = SDL_GetPerformanceFrequency();
}

PreciseTimer::~PreciseTimer()
{

}

void PreciseTimer::Start()
{
	is_enabled = true;
	start_time = SDL_GetPerformanceCounter();
	real_elapsed_time = TicksSinceStartup();
}

float PreciseTimer::Read()
{
	if (is_enabled)
	{
		delta_time = ((TicksSinceStartup() - real_elapsed_time) / frequency) * time_scale;
		elapsed_time += delta_time;
		real_elapsed_time = TicksSinceStartup();
	}

	return elapsed_time;
}

float PreciseTimer::Stop()
{
	is_enabled = false;
	//elapsed_time = (SDL_GetPerformanceCounter() - start_time) / frequency;

	LOG("PreciseTimer elapsed time: %f", elapsed_time);

	// Ha de tornar float ya veurem
	return elapsed_time;
}

float PreciseTimer::TicksSinceStartup() const
{
	return SDL_GetPerformanceCounter() - start_time;
}
