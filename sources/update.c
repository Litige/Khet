#include "world.h"
#include "button.h"
#include "errma.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

int		update_button(world_t *world, button_t *but);
int		update_elem(world_t *world, void *elem);

int		update_elem(world_t *world, void *elem)
{
	int	elem_status;
	//we cast it to button_t just to test obj_type
	//but it might as well be any type containing an obj_type
	switch (((button_t*)elem)->obj_type)
	{
		case BUTTON_T:
			elem_status = update_button(world, (button_t*)elem);
			break;
		default:
			elem_status = 0;
			fprintf(stderr, "Warning : unrecognized object type\n");
			break;
	}
	return(elem_status);
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

	if (world->click && but->overed)
	{
		if (!but->button_func(world))
		{
			return(1);
		}
	}

	if (but->txr == NULL)
	{
		fprintf(stderr,
			"Warning : missing texture on button at %dx%d\n",
			but->rect->x, but->rect->y);
	}
	else
	{
		if(SDL_RenderCopy(	world->renderer, but->txr,
					NULL, but->rect) < 0)
		{
			set_errma(SDL_ER);
			return(-1);
		}
	}

	return(0);
}
