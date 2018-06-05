#include <stdbool.h>

#include <SDL2/SDL.h>

#include "world.h"


void get_input(world_t *world)
{

	world->click	= false;
	world->bk_space	= false;
	world->enter	= false;

	if (SDL_IsTextInputActive())
	{
		memset(world->txt_input, 0, 32 * sizeof(char));
	}

	while (SDL_PollEvent(world->event_handler) == 1)
	{
		switch (world->event_handler->type)
		{
			case SDL_QUIT :
				world->running = false;
				break;
			case SDL_MOUSEBUTTONUP :
				world->click = true;
				break;
			case SDL_KEYDOWN :
				if (	world->event_handler->key.keysym.sym ==
					SDLK_BACKSPACE)
				{
					world->bk_space = true;
				}
				else if (
					world->event_handler->key.keysym.sym ==
					SDLK_RETURN  ||
					world->event_handler->key.keysym.sym ==
					SDLK_RETURN2 ||
					world->event_handler->key.keysym.sym ==
					SDLK_KP_ENTER)
				{
					world->enter = true;
				}
				break;
			case SDL_TEXTINPUT :
				strcat(	world->txt_input,
					world->event_handler->text.text);
				break;
		}

		SDL_GetMouseState(&world->mouse_pos[0], &world->mouse_pos[1]);
	}
}
