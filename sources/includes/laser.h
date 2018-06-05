#pragma once


#include <stdbool.h>

#include <SDL2/SDL.h>

#define NORM(orient) ((orient + 4) % 4)

typedef struct	laser_s laser_t;

#include "world.h"
#include "board.h"
#include "square.h"
#include "pawn.h"

struct 		laser_s
{
	SDL_Texture	*txr;
	SDL_Rect	*rect;

	square_t	*target;
	bool		kill;
};

laser_t	*new_laser(world_t *world, board_t *board);
bool	gen_laser_txr(world_t *world, board_t *board, laser_t *laser);
bool	draw_laser_square(	SDL_Surface	*surf,
				board_t		*board,
				SDL_Rect	*rect,
				int		laser_status[2]);
bool	gen_laser_square(	square_t	*square,
				int		(*laser_status)[2]);
void	delete_laser(laser_t *laser);
bool	get_starting_status(	board_t	*board,
				int	(*laser_status)[2],
				int	(*laser_pos)[2]);
bool	update_pos(int (*laser_pos)[2], int laser_status[2]);
bool	set_kill(square_t *square, int l_orient);
