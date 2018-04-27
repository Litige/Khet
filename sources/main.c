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

int		init(world_t *world);
void		quit(world_t *world);

int		main()
{
	world_t	*world;

	if (	new_world(&world) == EXIT_FAILURE ||
		init(world) == EXIT_FAILURE)
	{
		fprintf(stderr, "Error : %s\n", errma());
		return(EXIT_FAILURE);
	}

	printf("end of init\n");

	main_loop(world);

	printf("end of main loop\n");

	quit(world);
	return EXIT_SUCCESS;
}

int		init(world_t *world)
{
	world->running = true;
	world->size[0] = 720;
	world->size[1] = 1280;

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
	{
		set_errma(SDL_ER);
		fprintf(stderr, "Error : %s\n", errma());
		return(EXIT_FAILURE);
	}
	if (SDL_CreateWindowAndRenderer(
		world->size[1], world->size[0], 0,
		&(world->window), &(world->renderer)) < 0)
	{
		set_errma(SDL_ER);
		fprintf(stderr, "Error : %s\n", errma());
		SDL_Quit();
		return(EXIT_FAILURE);
	}
	if (
	SDL_SetRenderDrawBlendMode(world->renderer, SDL_BLENDMODE_BLEND) < 0 ||
	SDL_RenderClear(world->renderer) < 0)
	{
		set_errma(SDL_ER);
		fprintf(stderr, "Error : %s\n", errma());
		SDL_Quit();
		return(EXIT_FAILURE);
	}
	if (TTF_Init() < 0)
	{
		set_errma(TTF_ER);
		fprintf(stderr, "Error : %s\n", errma());
		SDL_Quit();
		return(EXIT_FAILURE);
	}
	if ((world->ttf_font = TTF_OpenFont(MAIN_FONT, MED_FONT)) == NULL)
	{
		set_errma(TTF_ER);
		fprintf(stderr, "error: %s\n", errma());
		TTF_Quit();
		SDL_Quit();
		return(EXIT_FAILURE);
	}

	return(EXIT_SUCCESS);
}

void		quit(world_t *world)
{
	SDL_DestroyRenderer(world->renderer);
	SDL_DestroyWindow(world->window);

	TTF_Quit();
	SDL_Quit();
}
