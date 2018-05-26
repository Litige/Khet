
#include "pawn_picker.h"
#include "images.h"
#include "object.h"
#include "world.h"
#include "button.h"
#include "square.h"
#include "errma.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

obj_t		*new_pawn_picker(world_t *world)
{
	pawn_picker_t	*new_pwp;
	obj_t		*new_obj;

	if (	(new_pwp	= malloc(sizeof(pawn_picker_t)))== NULL ||
		(new_pwp->rect	= malloc(sizeof(SDL_Rect))) 	== NULL ||
		(new_obj	= malloc(sizeof(obj_t)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_obj->obj		= new_pwp;
	new_obj->obj_type	= PAWN_PICKER_T;

	new_pwp->pawn_count[0][0] = 1;
	new_pwp->pawn_count[1][0] = 1;
	new_pwp->pawn_count[0][1] = 2;
	new_pwp->pawn_count[1][1] = 2;
	new_pwp->pawn_count[0][2] = 2;
	new_pwp->pawn_count[1][2] = 2;
	new_pwp->pawn_count[0][3] = 7;
	new_pwp->pawn_count[1][3] = 7;
	new_pwp->pawn_count[0][4] = 1;
	new_pwp->pawn_count[1][4] = 1;

	new_pwp->status = -1;

	new_pwp->rect->x = 5;
	new_pwp->rect->y = 5;
	new_pwp->rect->w = world->size[1] - (	15 +
						(world->size[0] - 10) +
						((world->size[0] - 10) / 4));
	new_pwp->rect->h = (((new_pwp->rect->w - 15) / 2 * 5) / 2) + 30;

	new_pwp->col = RED_PAWN;

	for (int i = 0; i < 10; i++)
	{
		new_pwp->but[i] = NULL;
	}

	for (int y = 0; y < 2 ; y++)
	{
		for (int x = 0; x < 5 ; x++)
		{
			new_pwp->but[x + (y * 5)] = new_button(
					world,
					type_pwn_picker_but(x, y),
					TXR_IMAGE,
					txr_path_pwn_picker_but(x, y),
					POS_X_IN_PICKER(new_pwp, y),
					POS_Y_IN_PICKER(new_pwp, x),
					TOPLEFT);

			if (!new_pwp->but[x + (y * 5)])
			{
				delete_pawn_picker(new_obj);
				return(NULL);
			}

			((button_t*)new_pwp->but[x + (y * 5)]->obj)->rect
				->w = PICKER_BUT_SIDE(new_pwp);
			((button_t*)new_pwp->but[x + (y * 5)]->obj)->rect
				->h = PICKER_BUT_SIDE(new_pwp);
		}
	}

	if (!set_pawn_but_color(world, new_pwp))
	{
		delete_pawn_picker(new_obj);
		return(NULL);
	}

	return(new_obj);
}

void		delete_pawn_picker(obj_t *pwp)
{
	if (pwp)
	{
		if (pwp->obj)
		{
			for (int i = 0; i < 10; i++)
			{
				if (((pawn_picker_t*)(pwp->obj))->but[i])
				{
					del_button(((pawn_picker_t*)(pwp->obj))
							->but[i]);
				}
			}
			free(((pawn_picker_t*)(pwp->obj))->rect);
			((pawn_picker_t*)(pwp->obj))->rect = NULL;
			free(pwp->obj);
			pwp->obj = NULL;
		}
		free(pwp);
		pwp = NULL;
	}
}

bool		set_pawn_but_color(world_t *world, pawn_picker_t *pwp)
{
	SDL_Surface	*surf;

	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
			case 0:		// PHARAOH
				surf = IMG_Load(PHARAOH_PAWN);
				break;
			case 1:		// SCARAB
				surf = IMG_Load(SCARAB_PAWN);
				break;
			case 2:		// ANUBIS
				surf = IMG_Load(ANUBIS_PAWN);
				break;
			case 3:		// PYRAMID
				surf = IMG_Load(PYRAMID_PAWN);
				break;
			case 4:		// SPHINX
				surf = IMG_Load(SPHINX_PAWN);
				break;
			default :
				return(false);
		}

		if (!surf)
		{
			set_errma(IMG_ER);
			return(false);
		}

		if (!swap_color(world, surf, pwp->col))
		{
			SDL_FreeSurface(surf);
			return(false);
		}

		SDL_DestroyTexture(((button_t*)(pwp->but[i]->obj))->txr);

		if ((((button_t*)(pwp->but[i]->obj))->txr =
			SDL_CreateTextureFromSurface(	world->renderer,
							surf)) == NULL)
		{
			set_errma(SDL_ER);
			SDL_FreeSurface(surf);
			return(false);
		}

		SDL_FreeSurface(surf);
	}

	return(true);
}

char		*txr_path_pwn_picker_but(int x, int y)
{
	switch (x + (y * 5))
	{
		case 0:
			return(PHARAOH_PAWN);
		case 1:
			return(SCARAB_PAWN);
		case 2:
			return(ANUBIS_PAWN);
		case 3:
			return(PYRAMID_PAWN);
		case 4:
			return(SPHINX_PAWN);
		case 5:
			return(RED_SQUARE_BUT);
		case 6:
			return(BLUE_SQUARE_BUT);
		case 7:
			return(R_ARROW_BUT);
		case 8:
			return(L_ARROW_BUT);
		case 9:
			return(DELETE_BUT);
		default :
			return("\0");
	}
}

button_type_t	type_pwn_picker_but(int x, int y)
{
	switch (x + (y * 5))
	{
		case 0:
			return(PP_PHARAOH);
		case 1:
			return(PP_SCARAB);
		case 2:
			return(PP_ANUBIS);
		case 3:
			return(PP_PYRAMID);
		case 4:
			return(PP_SPHINX);
		case 5:
			return(PP_RED_SQUARE);
		case 6:
			return(PP_BLUE_SQUARE);
		case 7:
			return(PP_R_ARROW);
		case 8:
			return(PP_L_ARROW);
		case 9:
			return(PP_DELETE);
		default :
			return(-1);
	}
}

int		valid_change(pawn_picker_t *picker, square_t *square)
{
	switch (picker->status)
	{
		case 0:		// PHARAOH
		case 1:		// SCARAB
		case 2:		// ANUBIS
		case 3:		// PYRAMID
		case 4:		// SPHINX
			if(	picker->col	!= square->type &&
				square->type	!= BLANK_SQUARE)
			{
				return(-1);
			}
			if (picker->pawn_count[picker->col][picker->status] > 0)
			{
				return(1);
			}
			else
			{
				return(-1);
			}
		case 7:		// R_ARROW
		case 8:		// L_ARROW
		case 9:		// DELETE
			if (square->pawn)
			{
				return(1);
			}
			else
			{
				return(0);
			}
		case 6:		// BLUE_SQUARE
		case 5:		// RED_SQUARE
		case -1:
		default :
			return(0);
	}

	return(0);
}

void		apply_picker_change(	world_t		*world,
					pawn_picker_t	*picker,
					square_t	*square)
{

	switch (picker->status)
	{
		case 0:		// PHARAOH
		case 1:		// SCARAB
		case 2:		// ANUBIS
		case 3:		// PYRAMID
		case 4:		// SPHINX
			picker->pawn_count[picker->col][picker->status] -= 1;
			if (square->pawn)
			{
				picker->pawn_count[square->pawn->color]
						[square->pawn->type - 1] += 1;
				delete_pawn(square->pawn);
				square->pawn = NULL;
			}
			square->pawn = new_pawn(world,
						picker->status + 1,
						NORTH,
						picker->col,
						square->rect);
			if (!square->pawn)
			{
				printf("an error occured at pawn creation\n");
			}
			break;
		case 7:		// R_ARROW
			if (square->pawn)
			{
				square->pawn->orient =
				(square->pawn->orient - 1) % 4;

				if (!set_pawn_txr(world, square->pawn))
				{
					picker->pawn_count
						[square->pawn->color]
						[square->pawn->type - 1] += 1;
					delete_pawn(square->pawn);
					square->pawn = NULL;
				}

			}
			break;
		case 8:		// L_ARROW
			square->pawn->orient =
			(square->pawn->orient + 1) % 4;

			if (!set_pawn_txr(world, square->pawn))
			{
				picker->pawn_count
					[square->pawn->color]
					[square->pawn->type - 1] += 1;
				delete_pawn(square->pawn);
				square->pawn = NULL;
			}
			break;
		case 9:		// DELETE
			if (square->pawn)
			{
				picker->pawn_count[square->pawn->color]
						[square->pawn->type - 1] += 1;
				delete_pawn(square->pawn);
				square->pawn = NULL;
			}
		case 6:		// BLUE_SQUARE
		case 5:		// RED_SQUARE
		case -1:
		default :
			break;
	}
}
