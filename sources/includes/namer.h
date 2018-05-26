#pragma once

#include "world.h"
#include "object.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct		namer_s namer_t;
struct			namer_s
{
	obj_t		*ok_but;	// create save then back to main menu
	obj_t		*back_but;	// back to map edit
	SDL_Rect	*rect;		//

	SDL_Texture	*txt_txr;
	SDL_Rect	*txt_rect;
	char		*name;	// max size (20 + 1) * 4 * sizeof(char)
				// for UTF-8 support
	int		ch_nb;	// max 20
	int		size_char[20];
};


obj_t	*new_namer(world_t *world);
bool	set_namer_txr_and_rect(world_t *world, namer_t *namer);
void	del_namer(obj_t *namer);
