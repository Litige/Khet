#pragma once

#include <stdbool.h>
#include "world.h"
#include "button.h"
#include "board.h"
#include "square.h"


int	update_elem(world_t *world, obj_t *elem);
int	update_button(world_t *world, button_t *but);
int	update_text(world_t *world, text_t *text);
int	update_map_edit(world_t *world, board_t *board);
int	update_board(world_t *world, board_t *board);
int	update_square_status(world_t *world, board_t *board, square_t *square);
int	draw_edit_square(world_t *world, board_t *board, square_t *square);
int	draw_board_square(world_t *world, board_t *board, square_t *square);
