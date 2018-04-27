#include <stdbool.h>

#include <SDL2/SDL.h>

#include "world.h"


void get_input(world_t *world)
{
	world->click = false;

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
		}

		SDL_GetMouseState(&world->mouse_pos[0], &world->mouse_pos[1]);
	}
}
