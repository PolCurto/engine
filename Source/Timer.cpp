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
	real_elapsed_time = start_time;
}

float Timer::Read()
{
	if (is_enabled)
	{
		delta_time = (TicksSinceStartup() - real_elapsed_time) * time_scale;
		elapsed_time += delta_time;
		real_elapsed_time = TicksSinceStartup();
	}

	return elapsed_time;
}

float Timer::Stop()
{
	is_enabled = false;
	//elapsed_time = SDL_GetTicks() - start_time;

	//LOG("Timer elapsed time: %f", static_cast<float>(elapsed_time) / 1000.0f);

	return elapsed_time;
}

float Timer::TicksSinceStartup() const
{
	return SDL_GetTicks() - start_time;
}