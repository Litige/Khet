#include "pawn.h"
#include "object.h"
#include "world.h"
#include "errma.h"
#include "images.h"
#include "colors.h"
#include "board.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>

pawn_t			*new_pawn(	world_t			*world,
					pawn_type_t		type,
					pawn_color_t		orient,
					pawn_orientation_t	color,
					SDL_Rect		*rect)
{
	pawn_t		*new_pwn;

	if (	(new_pwn	= malloc(sizeof(pawn_t))) 	== NULL ||
		(new_pwn->rect	= malloc(sizeof(SDL_Rect)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_pwn->type	= type;
	new_pwn->orient	= orient;
	new_pwn->color	= color;

	new_pwn->rect->w	= rect->w - 2;
	new_pwn->rect->h	= rect->h - 2;
	new_pwn->rect->x	= rect->x + 1;
	new_pwn->rect->y	= rect->y + 1;


	if (!set_pawn_txr(world, new_pwn))
	{
		delete_pawn(new_pwn);
		return(NULL);
	}

	return(new_pwn);
}

bool			set_pawn_txr(world_t *world, pawn_t *pawn)
{
	SDL_Surface	*surf;
	SDL_Surface	*tmp;

	pawn->txr = NULL;

	switch (pawn->type)
	{
		case PHARAOH:
			surf = IMG_Load(PHARAOH_PAWN);
			break;
		case SCARAB:
			surf = IMG_Load(SCARAB_PAWN);
			break;
		case ANUBIS:
			surf = IMG_Load(ANUBIS_PAWN);
			break;
		case PYRAMID:
			surf = IMG_Load(PYRAMID_PAWN);
			break;
		case SPHINX:
			surf = IMG_Load(SPHINX_PAWN);
			break;
		case NO_PAWN:
		default:
			return(false);

	}

	if (!surf)
	{
		set_errma(IMG_ER);
		return(false);
	}

	if (!swap_color(world, surf, pawn->color))
	{
		SDL_FreeSurface(surf);
		return(false);
	}

	tmp = surf;

	if (pawn->orient != NORTH)
	{
		if ((surf = rotateSurface90Degrees(surf, pawn->orient)) == NULL)
		{
			set_errma(GFX_ER);
			SDL_FreeSurface(surf);
			return(false);
		}
		else
		{
			SDL_FreeSurface(tmp);
		}
	}

	if ((pawn->txr = SDL_CreateTextureFromSurface(	world->renderer,
							surf)) == NULL)
	{
		set_errma(SDL_ER);
		SDL_FreeSurface(surf);
		return(false);
	}

	SDL_FreeSurface(surf);

	return(true);
}

void			delete_pawn(pawn_t *pawn)
{
	if (pawn)
	{
		if (pawn->txr)
		{
			SDL_DestroyTexture(pawn->txr);
			pawn->txr = NULL;
		}
		if (pawn->rect)
		{
			free(pawn->rect);
			pawn->rect = NULL;
		}
		free(pawn);
		pawn = NULL;
	}
}

int			display_pawn(world_t *world, pawn_t *pawn)
{
	if(SDL_RenderCopy(world->renderer, pawn->txr, NULL, pawn->rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}

	return(0);
}

void			swap_pawn_rect(pawn_t **pawn1, pawn_t **pawn2)
{
	SDL_Rect	*tmp;

	tmp		= (*pawn1)->rect;
	(*pawn1)->rect	= (*pawn2)->rect;
	(*pawn2)->rect	= tmp;
}

void			swap_pawn(pawn_t **pawn1, pawn_t **pawn2)
{
	pawn_t		*tmp;

	tmp	= *pawn1;
	*pawn1	= *pawn2;
	*pawn2	= tmp;
}

bool			swap_color(	world_t		*world,
					SDL_Surface	*surf,
					pawn_color_t	col)
{
	Uint32 		*t_pix;
	SDL_Color	tmp_i;

	if (SDL_LockSurface(surf) != 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	for (int y = 0; y < surf->w; y++)
	{
		for (int x = 0; x < surf->h; x++)
		{
			t_pix = (Uint8*) surf->pixels +
				y * surf->pitch +
				x * sizeof(*t_pix);
			if (	((SDL_Color*)t_pix)->g >
					(((SDL_Color*)t_pix)->r +
					((SDL_Color*)t_pix)->b) ||
				((SDL_Color*)t_pix)->g == 255)
			{
				if (col == RED_PAWN)
				{
					tmp_i.r = ((SDL_Color*)t_pix)->g;
					tmp_i.g = ((SDL_Color*)t_pix)->r;
					tmp_i.b = ((SDL_Color*)t_pix)->b;
				}
				else if (col == BLUE_PAWN)
				{
					tmp_i.r = ((SDL_Color*)t_pix)->r;
					tmp_i.g = ((SDL_Color*)t_pix)->b;
					tmp_i.b = ((SDL_Color*)t_pix)->g;
				}
				else
				{
					fprintf(stderr,
			"Warning : failed in color swapping (invalid color)\n");
					continue;
				}
				tmp_i.a = ((SDL_Color*)t_pix)->a;
				*t_pix = *((Uint32*)&tmp_i);
			}
		}
	}


	SDL_UnlockSurface(surf);

	return(true);
}
