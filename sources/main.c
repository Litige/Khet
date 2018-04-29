#include "world.h"
#include "errma.h"
#include "text.h"
#include "scene.h"
#include "input.h"
#include "button.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool		init(world_t *world);
void		quit(world_t *world);

int		main()
{
	world_t	*world;

	if (!new_world(&world) || !init(world))
	{
		fprintf(stderr, "Error : %s\n", errma());
		quit(world);
		return(false);
	}

	printf("end of init\n");

	printf("starting main loop\n");

	if (!main_loop(world))
	{
		fprintf(stderr, "Error : %s\n", errma());
	}

	printf("end of main loop\n");

	quit(world);
	return EXIT_SUCCESS;
}

bool		init(world_t *world)
{
	world->running = true;
	world->size[0] = 720;
	world->size[1] = 1280;

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (SDL_CreateWindowAndRenderer(
		world->size[1], world->size[0], 0,
		&(world->window), &(world->renderer)) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (
	SDL_SetRenderDrawBlendMode(world->renderer, SDL_BLENDMODE_BLEND) < 0 ||
	SDL_RenderClear(world->renderer) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (TTF_Init() < 0)
	{
		set_errma(TTF_ER);
		return(false);
	}

	if ((world->ttf_font = TTF_OpenFont(MAIN_FONT, MED_FONT)) == NULL)
	{
		set_errma(TTF_ER);
		return(false);
	}

	return(true);
}

void		quit(world_t *world)
{
	del_world(world);
	TTF_Quit();
	SDL_Quit();
}
