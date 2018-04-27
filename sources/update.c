#include "world.h"
#include "button.h"

#include <stdio.h>

#include <SDL2/SDL.h>

int		update_elem(world_t *world, void *elem)
{
	//we cast it to button_t just to test obj_type
	//but it might as well be any type containing an obj_type
	switch (((button_t*)elem)->obj_type)
	{
		case BUTTON_T:
			update_button(world, (button_t*)elem);
			break;
		default:
			fprintf(stderr, "Warning : unrecognized object type\n");
			break;
	}
}

int		update_button(world_t *world, button_t *but)
{
	if (SDL_PointInRect((SDL_Point*)world->mouse_pos, but->rect))
	{
		but->over_in_func(world,but);
		but->overed = true;
	}
	else
	{
		if (but->overed)
		{
			but->over_out_func(world,but);
			but->overed = false;
		}
	}
	printf("rect w = %i\nrect h = %i\nrect x = %i\nrect y = %i\n", but->rect->w, but->rect->h, but->rect->x, but->rect->y);
	SDL_RenderCopy(	world->renderer, but->txr, NULL, but->rect);
}
