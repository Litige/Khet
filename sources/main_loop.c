#include "world.h"
#include "scene.h"

#include "stdio.h"

bool		main_loop(world_t *world)
{

	if (new_scene(world, world->current_scene, MAIN_MENU_S) == false)
	{
		fprintf(stderr, "Error : %s\n", errma());
		return(false);
	}

	while (world->running)
	{
		get_input(world);
		if (!world->running)
		{
			break;
		}

		SDL_RenderClear(world->renderer);

		// printf("Starting elems update\n");
		for(int i = 0 ; world->current_scene->elems[i] != NULL ; i++)
		{
			printf("updating elem : %d\n", i);
			update_elem(world, world->current_scene->elems[i]);
		}

		SDL_RenderPresent(world->renderer);
	}

	return(true);
}
