
#include "world.h"
#include "board.h"
#include "square.h"
#include "object.h"
#include "errma.h"
#include "colors.h"
#include "draw.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

obj_t			*new_board(	world_t		*world,
					const char	map[8][10][4])
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

	new_obj->obj		= new_brd;
	new_obj->obj_type	= BOARD_T;

	if (world->type < PLY_VS_PLY_LOCAL || world->type >= NO_TYPE)
	{
		set_errma(BAD_ARG);
		delete_board(new_brd);
		return(NULL);
	}

	set_board_param(new_brd, world);


	new_brd->rect->h = world->size[0] - 10;
	new_brd->rect->w = (world->size[0] - 10) + ((world->size[0] - 10) / 4);

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

	// add rot_button

	return(new_obj);
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
			// free but
			// free laser
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
		world->type == NEW_CAMPAIGN	||
		world->type == LOAD_CAMPAIGN)
		&&
		(world->dif < EASY_DIF || world->dif >= NO_DIF))
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
	board->turn		= 0;
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
	if (SDL_MUSTLOCK(surf))
	{
		printf("must lock true\n");
	}

	if (SDL_SetColorKey(surf, SDL_TRUE, *((Uint32*)(&TRANPARENCY))) < 0)
	{
		set_errma(SDL_ER);
		printf("%s\n", errma());
		return(false);
	}

	if (SDL_MUSTLOCK(surf))
	{
		printf("must lock true\n");
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
				*((Uint32*)&RED) : *((Uint32*)&BLUE));
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
		printf("error on draw rect\n");
	}

	if ((board->txr = SDL_CreateTextureFromSurface(	world->renderer,
							surf)) == NULL)
	{
		set_errma(SDL_ER);
		SDL_FreeSurface(surf);
		return(false);
	}


	SDL_FreeSurface(surf);
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

void		empty_map(char map[8][10][4])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j][0] = 0;
			map[i][j][1] = 0;
			map[i][j][2] = 0;
			map[i][j][3] = 0;
		}
	}
}

bool	map_loading(world_t *world, char map[8][10][4])
{
	switch (world->type)
	{
		case PLY_VS_PLY_LOCAL :
		case PLY_VS_PLY_ONLINE :
		case PLY_VS_IA :
			// load map 1 for now
			// later choose map
			break;
		case NEW_CAMPAIGN :
			// load map according to level
			break;
		case LOAD_CAMPAIGN :
			// load autosave
			break;
		case EDIT_MAP :
			empty_map(map);
			break;
		case NO_TYPE :
		default :
			return(false);
	}

	return(true);
}

bool		save_map(board_t *brd, char *name)
{
	FILE	*file;
	int	error_val;
	char	*full_path;

	if (!(full_path = malloc(	sizeof(char) *
					(strlen(MAP_P) + 4 + strlen(name)))))
	{
		set_errma(MALLOC);
		return(false);
	}

	if (sprintf(full_path, "%s%s.map", MAP_P, name) < 0)
	{
		perror("error, couldn't save map");
		return(false);
	}

	error_val = 0;
	if (!(file = fopen(full_path, "w")))
	{
		perror("error, couldn't save map");
		return(false);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			error_val == fprintf(	file, "%i%i%i%i",
						brd->squares[i][j]->type,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->type	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->orient	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->color	: 0);
			if (error_val < 0)
			{
				perror("error, couldn't save map");
				return(false);
			}
		}
	}

	if (fclose(file) == EOF)
	{
		perror("error, couldn't save map");
		return(false);
	}

	return(true);
}

bool		load_map(char map[8][10][4], char *name)
{

}

void	set_square_layout(char map[8][10][4])
{
	for (int i = 0; i < 8; i++)
	{
		map[i][0][0] = 1;
		map[i][9][0] = 2;
		if (i == 0 || i == 7)
		{
			map[i][8][0] = 1;
			map[i][1][0] = 2;
		}
	}
}
