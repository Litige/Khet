#include "laser.h"
#include "object.h"
#include "world.h"
#include "board.h"
#include "square.h"
#include "pawn.h"
#include "colors.h"
#include "errma.h"

#include "stdbool.h"

#include <SDL2/SDL.h>

laser_t		*new_laser(world_t *world, board_t *board)
{
	laser_t	*laser;

	if (	!(laser		= malloc(sizeof(laser_t))) ||
		!(laser->rect	= malloc(sizeof(SDL_Rect))))
	{
		set_errma(MALLOC);
		return(NULL);
	}

	laser->rect->x = board->rect->x;
	laser->rect->y = board->rect->y;
	laser->rect->w = board->rect->w;
	laser->rect->h = board->rect->h;

	laser->txr	= NULL;
	laser->target	= NULL;
	laser->kill	= false;

	if (!gen_laser_txr(world, board, laser))
	{
		delete_laser(laser);
		return(NULL);
	}

	return(laser);
}

bool			gen_laser_txr(	world_t *world,
					board_t *board,
					laser_t *laser)
{
	SDL_Surface	*surf;
	int		laser_pos[2];
	int		laser_status[2]; // [0] = entré ; [1] = sorti
	bool		is_done;

	surf = SDL_CreateRGBSurface(	0, board->rect->w, board->rect->h, 32,
					r_mask, g_mask, b_mask, a_mask);

	if (!surf)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (SDL_SetColorKey(surf, SDL_TRUE, *((Uint32*)(&TRANPARENCY))) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (!get_starting_status(board, &laser_status, &laser_pos))
	{
		fprintf(stderr, "Warning : No laser on the board\n");
		return(false);
	}

	while (update_pos(&laser_pos, laser_status))
	{
		if (board->squares[laser_pos[0]][laser_pos[1]])
		{
			laser->target = board->squares	[laser_pos[0]]
			[laser_pos[1]];
			laser->kill = set_kill(	laser->target,
				NORM(laser_status[1] + 2));
		}
		if (laser->kill)
		{
			break;
		}

		if (	!gen_laser_square(	board->squares
							[laser_pos[0]]
							[laser_pos[1]],
						&laser_status)
		||
			!draw_laser_square(	surf, board,
						board->squares
							[laser_pos[0]]
							[laser_pos[1]]->rect,
						laser_status))
		{
			SDL_FreeSurface(surf);
			return(false);
		}

	}

	laser->txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);

	if (!laser->txr)
	{
		set_errma(SDL_ER);
		return(false);
	}

	return(true);
}

bool	set_kill(square_t *square, int l_orient)
{
	if (square->pawn)
	{
		switch (square->pawn->type)
		{
			case PYRAMID:
				if (	l_orient == square->pawn->orient ||
					l_orient ==
						NORM(square->pawn->orient + 1))
				{
					return(false);
				}
				else
				{
					return(true);
				}
			case PHARAOH:
				return(true);
			case ANUBIS:
				if (square->pawn->orient == l_orient)
				{
					return(false);
				}
				else
				{
					return(true);
				}
			case SPHINX:
			case SCARAB:
			default:
				return(false);
		}
	}

	return(false);
}

bool			draw_laser_square(	SDL_Surface	*surf,
						board_t		*board,
						SDL_Rect	*rect,
						int		laser_status[2])
{
	bool		result;
	int		start_x;
	int		start_y;
	int		end_x;
	int		end_y;

	if (laser_status[0] == -1 && laser_status[1] == -1)
	{
		return(true);
	}

	rect->x -= board->rect->x;
	rect->y -= board->rect->y;

	if (laser_status[0] == NORM(laser_status[1] - 2))
	{
		result = false;
		switch (laser_status[0])
		{
			case NORTH:
			case SOUTH:
				result = draw_s_line_to_surf(
					rect->x + rect->w / 2,
					rect->y,
					rect->x + rect->w / 2,
					rect->y + rect->h,
					surf,
					*((SDL_Color*)&RED));
				break;
			case EAST:
			case WEST:
			result = draw_s_line_to_surf(
					rect->x,
					rect->y + rect->h / 2,
					rect->x + rect->w,
					rect->y + rect->h / 2,
					surf,
					*((SDL_Color*)&RED));
				break;
			default:
				result = false;
				break;
		}

		rect->x += board->rect->x;
		rect->y += board->rect->y;

		return(result);
	}

	switch (laser_status[0])
	{
		case NORTH:
			start_x = rect->x + rect->w / 2;
			start_y = rect->y;
			break;
		case EAST:
			start_x = rect->x + rect->w;
			start_y = rect->y + rect->h / 2;
			break;
		case SOUTH:
			start_x = rect->x + rect->w / 2;
			start_y = rect->y + rect->h;
			break;
		case WEST:
			start_x = rect->x;
			start_y = rect->y + rect->h / 2;
			break;
		default:
			return(false);
	}

	switch (laser_status[1])
	{
		case NORTH:
			end_x = rect->x + rect->w / 2;
			end_y = rect->y;
			break;
		case EAST:
			end_x = rect->x + rect->w;
			end_y = rect->y + rect->h / 2;
			break;
		case SOUTH:
			end_x = rect->x + rect->w / 2;
			end_y = rect->y + rect->h;
			break;
		case WEST:
			end_x = rect->x;
			end_y = rect->y + rect->h / 2;
			break;
		default:
			return(false);
	}

	if (	!draw_s_line_to_surf(	start_x, start_y,
					rect->x + rect->w / 2,
					rect->y + rect->h / 2,
					surf, *((SDL_Color*)&RED)) ||
		!draw_s_line_to_surf(	rect->x + rect->w / 2,
					rect->y + rect->h / 2,
					end_x, end_y,
					surf, *((SDL_Color*)&RED)))
	{
		result = false;
	}
	else
	{
		result = true;
	}

	rect->x += board->rect->x;
	rect->y += board->rect->y;

	return(result);
}

bool			gen_laser_square(square_t *square, int (*laser_status)[2])
{
	int	l_orient;
	int	p_orient;

	if (	!square->pawn ||
		square->pawn->type == NO_PAWN)
	{
		(*laser_status)[0] = NORM((*laser_status)[1] + 2);
		return(true);
	}

	l_orient = NORM((*laser_status)[1] + 2);
	p_orient = square->pawn->orient;

	switch (square->pawn->type)
	{
		case PYRAMID:
			if (l_orient == p_orient)
			{
				l_orient = NORM(l_orient + 1);
				break;
			}
			else if (l_orient == NORM(p_orient + 1))
			{
				l_orient = NORM(l_orient - 1);
				break;
			}
			else
			{
				// passthrough
			}
		case SPHINX:
		case PHARAOH:
		case ANUBIS:
			(*laser_status)[0] = -1;
			(*laser_status)[1] = -1;
			return(true);
		case SCARAB:
			if (p_orient % 2 == 0)
			{
				l_orient += ((l_orient % 2 == 0) ? 1 : -1);
			}
			else
			{
				l_orient += ((l_orient % 2 == 0) ? -1 : 1);
			}
			l_orient = NORM(l_orient);
			break;
	}

	(*laser_status)[0] = NORM((*laser_status)[1] + 2);
	(*laser_status)[1] = l_orient;

	return(true);
}

void			delete_laser(laser_t *laser)
{
	if (laser)
	{
		if (laser->rect)
		{
			free(laser->rect);
			laser->rect = NULL;
		}
		if (laser->txr)
		{
			SDL_DestroyTexture(laser->txr);
			laser->txr = NULL;
		}
		laser->target = NULL;
	}
}

bool	update_pos(int (*laser_pos)[2], int laser_status[2])
{
	switch (laser_status[1])
	{
		case NORTH:
			(*laser_pos)[0] -= 1;
			break;
		case EAST:
			(*laser_pos)[1] += 1;
			break;
		case SOUTH:
			(*laser_pos)[0] += 1;
			break;
		case WEST:
			(*laser_pos)[1] -= 1;
			break;
		default:
			return(false);
	}

	if (	(*laser_pos)[0] <   0	||
		(*laser_pos)[0] >=  8	||
		(*laser_pos)[1] <   0	||
		(*laser_pos)[1] >= 10)
	{
		return(false);
	}

	return(true);
}

bool	get_starting_status(	board_t *board,
				int (*laser_status)[2],
				int (*laser_pos)[2])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (	board->squares[i][j]->pawn &&
				board->squares[i][j]->pawn->type == SPHINX &&
				board->squares[i][j]->pawn->color ==
				board->current_ply)
			{
				(*laser_status)[0] = NORM(board->squares[i][j]
							->pawn->orient + 2);
				(*laser_status)[1] = NORM(board->squares[i][j]
							->pawn->orient);
				(*laser_pos)[0] = board->squares[i][j]->pos[1];
				(*laser_pos)[1] = board->squares[i][j]->pos[0];
				return(true);
			}

		}
	}

	return(false);
}
