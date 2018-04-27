#include "world.h"
#include "errma.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

int		new_world(world_t **world)
{
	if (	(*world = malloc(sizeof(world_t))) == NULL ||
		((*world)->current_scene = malloc(sizeof(scene_t))) == NULL ||
		((*world)->event_handler = malloc(sizeof(SDL_Event))) == NULL)
	{
		set_errma(MALLOC);
		fprintf(stderr, "Error : %s\n", errma());
		return(EXIT_FAILURE);
	}


	(*world)->window	= NULL;
	(*world)->ttf_font	= NULL;
	(*world)->lang		= EN;
	(*world)->running	= true;
	(*world)->mouse_pos[0]	= 0;
	(*world)->mouse_pos[1]	= 0;
	(*world)->size[0]	= 0;
	(*world)->size[1]	= 0;

	return(EXIT_SUCCESS);
}
