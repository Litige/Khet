#pragma once


#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct			pawn_s pawn_t;

#include "world.h"
#include "board.h"

typedef enum	pawn_type_s pawn_type_t;
enum		pawn_type_s
{
	NO_PAWN,
	PHARAOH,
	SCARAB,
	ANUBIS,
	PYRAMID,
	SPHINX
};

typedef enum	pawn_color_s pawn_color_t;
enum		pawn_color_s
{
	RED_PAWN,
	BLUE_PAWN
};

typedef enum	pawn_orientation_s pawn_orientation_t;
enum		pawn_orientation_s
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct				pawn_s
{
	SDL_Rect		*rect;
	SDL_Texture		*txr;

	pawn_type_t		type;
	pawn_color_t		color;
	pawn_orientation_t	orient;
};

pawn_t	*new_pawn(	world_t			*world,
			pawn_type_t		type,
			pawn_color_t		orient,
			pawn_orientation_t	color,
			SDL_Rect		*rect);
bool	set_pawn_txr(world_t *world, pawn_t *pawn);
void	delete_pawn(pawn_t *pawn);
int	display_pawn(world_t *world, pawn_t *pawn);
void	swap_pawn(pawn_t **pawn1, pawn_t **pawn2);
void	swap_pawn_rect(pawn_t **pawn1, pawn_t **pawn2);
bool	swap_color(	world_t		*world,
			SDL_Surface	*surf,
			pawn_color_t	col);
