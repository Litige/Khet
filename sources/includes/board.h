#pragma once

#include "object.h"
#include "button.h"

#include <SDL2/SDL.h>

typedef enum 	player_e player_t;
enum 		player_e
{
	RED_PLY,
	BLUE_PLY
};

#define TARGET_PAWN(board)  	board->squares\
				[board->target_pos[1]]\
				[board->target_pos[0]]->pawn

typedef struct		board_s board_t;

#include "laser.h"
#include "square.h"
#include "pawn.h"

struct			board_s
{
	square_t	*squares[8][10];
	obj_t		*rot_button[2];
	laser_t		*laser;

	SDL_Rect	*rect;
	SDL_Texture	*txr;			// contains colored squares
						// never change once created

	difficulty_t	dif;
	game_type_t	type;
	camp_level_t	level;

	player_t	current_ply;
	int		turn;
	int		target_pos[2];		// {x, y} ; board->square[x][y]

	bool		in_anime;
	int		frame_nb;
};

obj_t	*new_board(world_t *world, char map[8][10][4]);
void	delete_board(obj_t *board);
void 	set_board_param(board_t *board, world_t* world);
bool	new_board_txr(world_t *world, board_t *board);
void	reset_pawn_rect(board_t *board, pawn_t *pawn, int i, int j);
bool	valid_quit(world_t *world, board_t *board);
void	set_but_pos(board_t *board);
