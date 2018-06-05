
#include "square.h"
#include "errma.h"
#include "world.h"
#include "board.h"
#include "pawn.h"

#include <SDL2/SDL.h>

square_t		*new_square(	world_t	*world,
					char	conf[4],
					int	y,
					int	x,
					board_t	*board)
{
	square_t	*new_sqr;

	if (	(new_sqr 	= malloc(sizeof(board_t))) 	== NULL ||
		(new_sqr->rect	= malloc(sizeof(SDL_Rect))) 	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_sqr->status	= 0;
	new_sqr->pos[0] = x;
	new_sqr->pos[1] = y;
	new_sqr->rect->w = board->rect->h / 8;
	new_sqr->rect->h = board->rect->h / 8;
	new_sqr->rect->x = board->rect->x + (board->rect->h / 8 * x);
	new_sqr->rect->y = board->rect->y + (board->rect->h / 8 * y);

	new_sqr->type = conf[0];

	if (conf[1] != NO_PAWN)
	{
		new_sqr->pawn = new_pawn(	world,
						conf[1], conf[2], conf[3],
						new_sqr->rect);
		if (!new_sqr->pawn)
		{
			delete_square(new_sqr);
			return(NULL);
		}
	}
	else
	{
		new_sqr->pawn = NULL;
	}

	return(new_sqr);
}

void			delete_square(square_t *square)
{
	if (square)
	{
		if (square->pawn)
		{
			delete_pawn(square->pawn);
			square->pawn = NULL;
		}

		free(square->rect);
		square->rect = NULL;

		free(square);
		square = NULL;
	}
}

void			set_all_move_target_flag(board_t *board, bool flag)
{
	for (	int i = board->target_pos[1] - 1;
		i <= board->target_pos[1] + 1;
		i++)
	{
		for (	int j = board->target_pos[0] - 1;
			j <= board->target_pos[0] + 1;
			j++)
		{

			if (	(j == board->target_pos[0] &&
				i == board->target_pos[1]) ||
				0 > i || i >= 8 ||
				0 > j || j >= 10)
			{
				continue;
			}
			if (flag && valid_move(board, i, j))
			{
				ADD_FLAG(	board->squares[i][j]->status,
						MOVE_TARGET);
			}
			else if (!flag)
			{
				DEL_FLAG(	board->squares[i][j]->status,
						MOVE_TARGET);
			}
		}
	}
}

void			set_all_flag_to_no(board_t *board)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board->squares[i][j]->status = 0;
		}
	}

}

bool			valid_move(board_t *board, int i, int j)
{
	square_t	*target_square;
	square_t	*move_target;

	target_square	= board->squares	[board->target_pos[1]]
						[board->target_pos[0]];
	move_target	= board->squares[i][j];

	if (	move_target->type != BLANK_SQUARE &&
		move_target->type != board->current_ply)
	{
		return(false);
	}

	switch (target_square->pawn->type)
	{
		case PHARAOH:
		case ANUBIS:
		case PYRAMID:
			if (	!move_target->pawn &&
				(move_target->type ==
				target_square->pawn->color ||
				move_target->type == BLANK_SQUARE))
			{
				return(true);
			}
			break;
		case SCARAB:
			if (move_target->pawn)
			{
				if (	move_target->pawn->type == ANUBIS ||
					move_target->pawn->type == PYRAMID)
				{
					return(true);
				}
				break;
			}
			return(true);
		case SPHINX:
		default:
			return(false);
	}

	return(false);
}
