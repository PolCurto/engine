#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui_impl_sdl2.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{
    delete[] mouse_buttons;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
    mouse_buttons = new bool[3];
    for (int i = 0; i < 3; i++)
        mouse_buttons[i] = false;

	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;
    mouse_motion_x = 0;
    mouse_motion_y = 0;
    mouse_wheel = 0;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;

            case SDL_WINDOWEVENT:
                if (sdlEvent.window.windowID == SDL_GetWindowID(App->GetWindow()->window) && (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
                    App->GetWindow()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);                    
                break;

            case SDL_MOUSEMOTION:
                mouse_motion_x = static_cast<float>(sdlEvent.motion.xrel);
                mouse_motion_y = static_cast<float>(sdlEvent.motion.yrel);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    mouse_buttons[LEFT_BUTTON] = true;
                else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    mouse_buttons[RIGHT_BUTTON] = true;
                else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                    mouse_buttons[MIDDLE_BUTTON] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    mouse_buttons[LEFT_BUTTON] = false;
                else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    mouse_buttons[RIGHT_BUTTON] = false;
                else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                    mouse_buttons[MIDDLE_BUTTON] = false;
                break;

            case SDL_MOUSEWHEEL:
                mouse_wheel = sdlEvent.wheel.y;
                break;
        }
    }
    //LOG("Mouse motion x: %f, y: %f", mouse_motion_x, mouse_motion_y);
    keyboard = SDL_GetKeyboardState(NULL);


    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
