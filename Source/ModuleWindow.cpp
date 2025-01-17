#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEditorCamera.h"
#include "SDL.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		SDL_DisplayMode display;
		SDL_GetDesktopDisplayMode(0, &display);
		window_width = display.w / 2;
		window_height = display.h / 2;

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}


	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::WindowResized(const unsigned int width, const unsigned int height)
{
	window_width = width;
	window_height = height;

	App->GetCamera()->SetAspectRatio(static_cast<float>(window_width) / static_cast<float>(window_height));
}

float ModuleWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetBrightness(float brightness) const
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWidth(const unsigned int width)
{
	window_width = width;
	SDL_SetWindowSize(window, window_width, window_height);
}

void ModuleWindow::SetHeight(const unsigned int height)
{
	window_height = height;
	SDL_SetWindowSize(window, window_width, window_height);
}

bool ModuleWindow::SetFullscreen(bool fullscreen) const
{
	return SDL_SetWindowFullscreen(window, fullscreen);
}

void ModuleWindow::SetResizable(bool resizable) const
{
	SDL_SetWindowResizable(window, resizable ? SDL_TRUE : SDL_FALSE);
}

void ModuleWindow::SetBorderless(bool borderless) const
{
	SDL_SetWindowBordered(window, borderless ? SDL_FALSE : SDL_TRUE);
}

bool ModuleWindow::SetFullDesktop(bool full_desktop) const
{
	return SDL_SetWindowFullscreen(window, full_desktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
