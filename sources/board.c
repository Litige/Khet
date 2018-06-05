
#include "world.h"
#include "board.h"
#include "square.h"
#include "object.h"
#include "errma.h"
#include "colors.h"
#include "draw.h"
#include "images.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

obj_t			*new_board(	world_t		*world,
					char	map[8][10][4])
{
	board_t		*new_brd;
	obj_t		*new_obj;

	if (	(new_brd 	= malloc(sizeof(board_t))) 	== NULL ||
		(new_brd->rect	= malloc(sizeof(SDL_Rect))) 	== NULL ||
		(new_obj	= malloc(sizeof(obj_t)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	for (int i = 0; i < 8; i++)
	{
		memset(new_brd->squares[i], 0, 10 * sizeof(square_t*));
	}

	new_brd->txr	= NULL;
	new_brd->laser	= NULL;

	new_obj->obj		= new_brd;
	new_obj->obj_type	= BOARD_T;

	if (world->type < PLY_VS_PLY_LOCAL || world->type >= NO_TYPE)
	{
		set_errma(BAD_ARG);
		delete_board(new_brd);
		return(NULL);
	}

	set_board_param(new_brd, world);

	if (world->type == LOAD_CAMPAIGN)
	{
		if (!load_save(new_brd, map))
		{
			return(NULL);
		}
	}

	new_brd->rect->h = world->size[0] - 10;
	new_brd->rect->w = (world->size[0] - 10) + ((world->size[0] - 10) / 4);

	// make sure	new_brd->rect->h % 8  == 0
	// and		new_brd->rect->w % 10 == 0
	new_brd->rect->h = (new_brd->rect->h / 8) * 8;
	new_brd->rect->w = (new_brd->rect->w / 10) * 10;

	new_brd->rect->x = world->size[1] - (new_brd->rect->w + 5);
	new_brd->rect->y = 5;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			new_brd->squares[i][j] = new_square(	world,
								map[i][j],
								i, j,
								new_brd);
			if (!(new_brd->squares[i][j]))
			{
				delete_board(new_obj);
				return(NULL);
			}
		}
	}

	new_brd->txr = NULL;
	if (!new_board_txr(world, new_brd))
	{
		delete_board(new_obj);
		return(NULL);
	}

	new_brd->rot_button[0] = new_button(	world, BRD_ROT_R, TXR_IMAGE,
						R_ARROW_BUT, 0, 0, TOPLEFT);
	new_brd->rot_button[1] = new_button(	world, BRD_ROT_L, TXR_IMAGE,
						L_ARROW_BUT, 0, 0, TOPLEFT);

	if (!new_brd->rot_button[0] || !new_brd->rot_button[1])
	{
		delete_board(new_obj);
		return(NULL);
	}

	((button_t*)(new_brd->rot_button[0]->obj))->rect->w =
					new_brd->squares[0][0]->rect->w / 4;
	((button_t*)(new_brd->rot_button[0]->obj))->rect->h =
	 				new_brd->squares[0][0]->rect->w / 4;
	((button_t*)(new_brd->rot_button[1]->obj))->rect->w =
	 				new_brd->squares[0][0]->rect->w / 4;
	((button_t*)(new_brd->rot_button[1]->obj))->rect->h =
	 				new_brd->squares[0][0]->rect->w / 4;

	return(new_obj);
}

void			set_but_pos(board_t *board)
{
	((button_t*)(board->rot_button[0]->obj))->rect->x =
			TARGET_PAWN(board)->rect->x +
			TARGET_PAWN(board)->rect->w -
			((button_t*)(board->rot_button[1]->obj))->rect->w;
	((button_t*)(board->rot_button[0]->obj))->rect->y =
			TARGET_PAWN(board)->rect->y +
			TARGET_PAWN(board)->rect->h -
			((button_t*)(board->rot_button[1]->obj))->rect->h;
	((button_t*)(board->rot_button[1]->obj))->rect->x =
			TARGET_PAWN(board)->rect->x;
	((button_t*)(board->rot_button[1]->obj))->rect->y =
			TARGET_PAWN(board)->rect->y +
			TARGET_PAWN(board)->rect->h -
			((button_t*)(board->rot_button[1]->obj))->rect->h;
}

void			delete_board(obj_t *board)
{
	if (board)
	{
		if (board->obj)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					delete_square(
						((board_t*)(board->obj))
						->squares[i][j]);
				}
			}
			if (((board_t*)(board->obj))->rot_button[0])
			{
				del_button(	((board_t*)(board->obj))
						->rot_button[0]);
				((board_t*)(board->obj))->rot_button[0] = NULL;
			}
			if (((board_t*)(board->obj))->rot_button[1])
			{
				del_button(	((board_t*)(board->obj))
						->rot_button[1]);
				((board_t*)(board->obj))->rot_button[1] = NULL;
			}
			if (((board_t*)(board->obj))->laser)
			{
				delete_laser(((board_t*)(board->obj))->laser);
				((board_t*)(board->obj))->laser = NULL;
			}
			free(((board_t*)(board->obj))->rect);
			((board_t*)(board->obj))->rect = NULL;
			SDL_DestroyTexture(((board_t*)(board->obj))->txr);
			((board_t*)(board->obj))->txr = NULL;
			free(board->obj);
		}
		free(board);
	}
}

void 			set_board_param(	board_t		*board,
						world_t		*world)
{
	if (	(world->type == PLY_VS_IA	||
		world->type == NEW_CAMPAIGN)
		&&
		(	world->dif != EASY_DIF &&
			world->dif != MED_DIF  &&
			world->dif != HARD_DIF))
	{
		fprintf(stderr,
			"Warning : bad difficulty argument for new board\n");
		fprintf(stderr,
			"setting difficulty to easy\n");
		board->dif = EASY_DIF;
	}
	else
	{
		board->dif = world->dif;
	}

	board->type		= world->type;
	board->level		= world->level;
	board->turn		= 1;
	board->current_ply	= RED_PLY;
	board->target_pos[0]	= -1;
	board->target_pos[1]	= -1;
	board->in_anime		= false;
	board->frame_nb		= -1;
}

bool			new_board_txr(world_t *world, board_t *board)
{
	SDL_Surface	*surf;
	int		tmp;

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

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			(board->squares[i][j])->rect->x -= board->rect->x;
			(board->squares[i][j])->rect->y -= board->rect->y;
			if (board->squares[i][j]->type != BLANK_SQUARE)
			{
				tmp = SDL_FillRect(
						surf,
						board->squares[i][j]->rect,
				(board->squares[i][j]->type == RED_SQUARE) ?
				*((Uint32*)&ST_RED) : *((Uint32*)&ST_BLUE));
			}
			else
			{
				tmp = SDL_FillRect(
						surf,
						board->squares[i][j]->rect,
						*((Uint32*)&BLACK));
			}
			board->squares[i][j]->rect->x += board->rect->x;
			board->squares[i][j]->rect->y += board->rect->y;
			if (tmp < 0)
			{
				set_errma(SDL_ER);
				SDL_FreeSurface(surf);
				return(false);
			}
		}
	}

	if (!draw_rect_to_surf(	board->rect,
				surf,
				board->rect,
				*((Uint32*)(&GOLD))))
	{
		SDL_FreeSurface(surf);
		return(false);
	}

	board->txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);

	if (board->txr == NULL)
	{
		set_errma(SDL_ER);
		return(false);
	}

	return(true);
}

bool		valid_click(board_t *board, square_t *target)
{
	if (board->target_pos[0] == -1 || board->target_pos[1] == -1)
	{
		if (target->pawn && target->pawn->type != SPHINX)
		{
			return(board->current_ply == target->pawn->color);
		}
	}
	else
	{
		if (IS_SET_FLAG(target->status, MOVE_TARGET))
		{
			return(true);
		}
	}

	return(false);
}

bool		valid_quit(world_t *world, board_t *board)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (SDL_PointInRect(	(SDL_Point*)world->mouse_pos,
						board->squares[i][j]->rect))
			{
				return(IS_SET_FLAG(
					board->squares[i][j]->status,
					MOVE_TARGET));
			}
		}
	}

	return(true);
}

void		reset_pawn_rect(board_t *board, pawn_t *pawn, int i, int j)
{
	pawn->rect->x = board->squares[i][j]->rect->x + 1;
	pawn->rect->y = board->squares[i][j]->rect->y + 1;
	pawn->rect->w = board->squares[i][j]->rect->w - 2;
	pawn->rect->h = board->squares[i][j]->rect->h - 2;
}
