#pragma once
#include "Module.h"

typedef unsigned __int8 Uint8;

typedef enum
{
	LEFT_BUTTON = 0,
	RIGHT_BUTTON = 1,
	MIDDLE_BUTTON = 2
} mouse_buttons;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	const Uint8* GetKeyboard() const { return keyboard; }
	const bool* GetMouseButtons() const { return mouse_buttons; }
	const float GetMouseMotionX() const { return mouse_motion_x; }
	const float GetMouseMotionY() const { return mouse_motion_y; }
	const float GetMouseWheel() const { return mouse_wheel; }

private:
	const Uint8 *keyboard = NULL;

	float mouse_motion_x = 0;
	float mouse_motion_y = 0;

	bool* mouse_buttons;
	float mouse_wheel = 0;
};