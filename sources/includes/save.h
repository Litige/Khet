#pragma once

#include "world.h"
#include "board.h"

bool	map_loading(world_t *world, char map[8][10][4]);

bool	save_map(board_t *brd, char *name);
bool	load_map(char map[8][10][4], char *name);

bool	save_game(board_t *brd);
bool	load_save(board_t *board, char map[8][10][4]);

void	set_square_layout(char map[8][10][4]);
void	empty_map(char map[8][10][4]);
