#include "Globals.h"
#include "Application.h"
#include "Timer.h"
#include "SDL.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::Start()
{
	is_enabled = true;
	start_time = SDL_GetTicks();

	LOG("Timer start time: %d", start_time);
}

unsigned int Timer::Read()
{
	if (is_enabled)
		elapsed_time = SDL_GetTicks() - start_time;

	return elapsed_time;
}

unsigned int Timer::Stop()
{
	is_enabled = false;
	elapsed_time = SDL_GetTicks() - start_time;

	LOG("Timer elapsed time: %f", static_cast<float>(elapsed_time) / 1000.0f);

	return elapsed_time;
}