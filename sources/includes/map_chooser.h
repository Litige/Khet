#pragma once

#include "world.h"
#include "object.h"
#include "button.h"

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

typedef struct			map_chooser_s map_chooser_t;
struct				map_chooser_s
{
	SDL_Texture		**maps_txr;
	char			**maps_name;

	obj_t			*up_but;
	obj_t			*dw_but;

	SDL_Rect		*txr_rect;
	SDL_Rect		*select_rect;

	SDL_Rect		*names_rect[5];
	SDL_Texture		*names_txr[5];

	int			target;
	Uint8			top_map;
	Uint8			map_nb;

};


obj_t		*new_map_chooser(world_t *world);

bool		set_all_rect(world_t *world, map_chooser_t *map_chooser);
bool		set_all_names(map_chooser_t *map_chooser);
bool		set_all_txr(world_t *world, map_chooser_t *map_chooser);
bool		set_names_txr_and_rect(	world_t		*world,
					map_chooser_t	*map_chooser);

Uint8		get_map_number();
SDL_Texture	*get_txr_from_map(	world_t		*world,
					map_chooser_t	*map_chooser,
					const char	map[8][10][4]);
bool		is_a_map(char *filename);
void		del_map_chooser(obj_t *but);
int		blit_pawn_to_surf(	world_t			*world,
					SDL_Surface		*surf,
					SDL_Rect		*rect,
					pawn_type_t		type,
					pawn_color_t		color,
					pawn_orientation_t	orient);
