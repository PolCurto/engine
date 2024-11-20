#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void WindowResized(unsigned width, unsigned height);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	float GetBrightness() const { return SDL_GetWindowBrightness(window); }

	void SetBrightness(float brightness) const;
	void SetWidth(int width) const;
	void SetHeight(int height) const;

	bool SetFullscreen(bool fullscreen) const;
	void SetResizable(bool resizable) const;
	void SetBorderless(bool borderless) const;
	bool SetFullDesktop(bool full_desktop) const;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	int width;
	int height;
};

#endif // __ModuleWindow_H__