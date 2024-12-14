#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

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

	void WindowResized(const unsigned int width, const unsigned int height);

	int GetWidth() const { return window_width; }
	int GetHeight() const { return window_height; }

	float GetBrightness() const;

	void SetBrightness(const float brightness) const;
	void SetWidth(const unsigned int width);
	void SetHeight(const unsigned int height);

	bool SetFullscreen(bool fullscreen) const;
	void SetResizable(bool resizable) const;
	void SetBorderless(bool borderless) const;
	bool SetFullDesktop(bool full_desktop) const;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

private:
	int window_width;
	int window_height;
};

#endif // __ModuleWindow_H__