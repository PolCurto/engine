#ifndef __MODULE_HARDWARE_H__
#define __MODULE_HARDWAR_H__

#include "Module.h"
#include <string>

struct SDL_version;

class ModuleHardware : public Module
{
public:

	ModuleHardware();
	~ModuleHardware();

	bool Init();
	update_status Update();
	bool CleanUp();

	void ShowHardwareInfo() const;

private:
	SDL_version* sdl_version;

};

#endif // __MODULE_HARDWARE_H__

