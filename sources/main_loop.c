#include "world.h"
#include "scene.h"
#include "errma.h"

#include <stdio.h>

bool		main_loop(world_t *world)
{
	int elem_status;

	if (new_scene(world, world->current_scene, MAIN_MENU_S) == false)
	{
		return(false);
	}

	while (world->running)
	{
		get_input(world);
		if (!world->running)
		{
			break;
		}

		if (SDL_RenderClear(world->renderer) < 0)
		{
			set_errma(SDL_ER);
			return(false);
		}

		// printf("Starting elems update\n");
		for(int i = 0 ; world->current_scene->elems[i] != NULL ; i++)
		{
			elem_status = update_elem(world,
					world->current_scene->elems[i]);
			if (elem_status < 0)
			{
				return(false);
			}
			else if (elem_status > 0)
			{
				break;
			}
		}

		SDL_RenderPresent(world->renderer);
	}

	return(true);
}
