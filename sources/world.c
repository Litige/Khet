#include "world.h"
#include "errma.h"
#include "text.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool		new_world(world_t **world)
{
	if (	(*world = malloc(sizeof(world_t))) == NULL ||
		((*world)->current_scene = malloc(sizeof(scene_t))) == NULL ||
		((*world)->event_handler = malloc(sizeof(SDL_Event))) == NULL)
	{
		set_errma(MALLOC);
		fprintf(stderr, "Error : %s\n", errma());
		return(false);
	}

	(*world)->current_scene->elems	= NULL;
	(*world)->window		= NULL;
	(*world)->ttf_font		= NULL;
	(*world)->map			= NULL;
	(*world)->lang			= EN;
	(*world)->dif			= NO_DIF;
	(*world)->type			= NO_TYPE;
	(*world)->level			= NO_LV;
	(*world)->winner		= -1;
	(*world)->running		= true;
	(*world)->fullscreen		= false;
	(*world)->mouse_pos[0]		= 0;
	(*world)->mouse_pos[1]		= 0;
	(*world)->size[0]		= 0;
	(*world)->size[1]		= 0;
	(*world)->click			= false;
	(*world)->bk_space		= false;
	(*world)->enter			= false;
	memset((*world)->txt_input, 0, 32 * sizeof(char));

	return(true);
}

void		del_world(world_t *world)
{
	if (world)
	{
		if (world->event_handler)
		{
			free(world->event_handler);
			world->event_handler = NULL;
		}
		if (world->ttf_font)
		{
			TTF_CloseFont(world->ttf_font);
			world->ttf_font = NULL;
		}
		if (world->window)
		{
			SDL_DestroyWindow(world->window);
			world->window = NULL;
		}
		if (world->renderer)
		{
			SDL_DestroyRenderer(world->renderer);
			world->renderer = NULL;
		}
		if (world->current_scene)
		{
			del_scene(world->current_scene);
			free(world->current_scene);
			world->current_scene = NULL;
		}

		free(world);
		world == NULL;
	}
}

bool		set_font_size(world_t *world, int font_size)
{
	TTF_CloseFont(world->ttf_font);
	if ((world->ttf_font = TTF_OpenFont(MAIN_FONT, font_size)) == NULL)
	{
		set_errma(TTF_ER);
		return(false);
	}
	return(true);
}


bool		is_txt_input(world_t *world)
{
	for (int i = 0; i < 32; i++)
	{
		if (world->txt_input[i] != 0)
		{
			return(true);
		}
	}

	return(false);
}
