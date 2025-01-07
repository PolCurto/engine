#include "ModuleTime.h"

ModuleTime::ModuleTime()
{
	timer = std::make_unique<Timer>();
	precise_timer = std::make_unique<PreciseTimer>();
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::PreInit()
{
	return false;
}

bool ModuleTime::Init()
{
	return false;
}

update_status ModuleTime::PreUpdate()
{
	return update_status();
}

update_status ModuleTime::Update()
{
	return update_status();
}

bool ModuleTime::CleanUp()
{
	return false;
}
