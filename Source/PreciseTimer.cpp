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

	LOG("Precise timer start time: %d", start_time);
}

unsigned int PreciseTimer::Read()
{
	if (is_enabled)
		elapsed_time = (SDL_GetPerformanceCounter() - start_time) / frequency;

	return elapsed_time;
}

unsigned int PreciseTimer::Stop()
{
	is_enabled = false;
	elapsed_time = (SDL_GetPerformanceCounter() - start_time) / frequency;

	LOG("PreciseTimer elapsed time: %f", elapsed_time);

	return elapsed_time;
}