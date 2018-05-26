#pragma once

#include "world.h"
#include "pawn.h"

#include <SDL2/SDL.h>

#define SWITCH_FLAG(x, y)	(x ^=  y)
#define ADD_FLAG(x, y)		(x |=  y)
#define DEL_FLAG(x, y)		(x &= ~y)
#define IS_SET_FLAG(x, y)	(x &   y)

typedef enum	square_color_s square_color_t;
enum		square_color_s
{
	RED_SQUARE,
	BLUE_SQUARE,
	BLANK_SQUARE
};

typedef enum	square_status_s square_status_t;
enum		square_status_s
{
	OVERED		= (1 << 0),
	CLICKED		= (1 << 1),
	MOVE_TARGET	= (1 << 2)
};

typedef struct		square_s square_t;
struct			square_s
{
	SDL_Rect	*rect;

	pawn_t		*pawn;
	square_color_t	type;
	int		status;
	int		pos[2];		// {x, y} ; board->square[x][y]
};

#include "board.h"

square_t		*new_square(	world_t	*world,
					char	conf[4],
					int	x,
					int	y,
					board_t	*board);
void			delete_square(square_t *square);
void			set_all_move_target_flag(board_t *board, bool flag);
void			set_all_flag_to_no(board_t *board);
bool			valid_move(board_t *board, int i, int j);
