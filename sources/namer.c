
#include "namer.h"
#include "object.h"
#include "button.h"
#include "colors.h"
#include "errma.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

obj_t		*new_namer(world_t *world)
{
	namer_t	*new_nmr;
	obj_t	*new_obj;

	if (	(new_nmr = malloc(sizeof(namer_t))) 		== NULL ||
		(new_nmr->rect = malloc(sizeof(SDL_Rect)))	== NULL ||
		(new_nmr->txt_rect = malloc(sizeof(SDL_Rect)))	== NULL ||
		(new_obj = malloc(sizeof(obj_t))) 		== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_obj->obj		= new_nmr;
	new_obj->obj_type	= NAMER_T;

	// 20 UTF8 char + spare space for \0
	if ((new_nmr->name = malloc(21 * 4 * sizeof(char))) == NULL)
	{
		set_errma(MALLOC);
		del_namer(new_obj);
		return(NULL);
	}

	memset(new_nmr->name, 0, 21 * 4 * sizeof(char));
	memset(new_nmr->size_char, 0, 20 * sizeof(int));
	new_nmr->ch_nb		= 0;

	new_nmr->rect->w = world->size[1] / 2;
	new_nmr->rect->h = world->size[0] / 4;
	new_nmr->rect->y = world->size[0] / 2 - new_nmr->rect->h / 2;
	new_nmr->rect->x = world->size[1] / 2 - new_nmr->rect->w / 2;

	new_nmr->txt_rect->x = new_nmr->rect->x + (new_nmr->rect->w / 2);
	new_nmr->txt_rect->y = new_nmr->rect->y + (new_nmr->rect->h / 3);


	new_nmr->ok_but = new_button(	world,
					NAMER_OK,
					TXR_TEXT,
					TEXT[world->lang][B_NAMER_OK],
					new_nmr->rect->x +
					(new_nmr->rect->w / 4 * 3),
					new_nmr->rect->y +
					(new_nmr->rect->h / 3 * 2),
					CENTER);
	new_nmr->back_but = new_button(	world,
					NAMER_BACK,
					TXR_TEXT,
					TEXT[world->lang][B_NAMER_BACK],
					new_nmr->rect->x +
					(new_nmr->rect->w / 4),
					new_nmr->rect->y +
					(new_nmr->rect->h / 3 * 2),
					CENTER);

	if (!new_nmr->ok_but || !new_nmr->back_but)
	{
		del_namer(new_obj);
		return(NULL);
	}


	if (!set_namer_txr_and_rect(world, new_nmr))
	{

		del_namer(new_obj);
		return(NULL);
	}


	return(new_obj);
}

bool			set_namer_txr_and_rect(world_t *world, namer_t *namer)
{
	SDL_Surface	*surf;
	int		w;

	namer->txt_txr = NULL;

	if (TTF_SizeUTF8(world->ttf_font, namer->name, &w, NULL) < 0)
	{
		set_errma(TTF_ER);
		return(false);
	}

	if (w == 0)
	{
		namer->txt_rect->x = namer->rect->x + (namer->rect->w / 2);
		namer->txt_rect->y = namer->rect->y + (namer->rect->h / 3);
		namer->txt_rect->w = 0;
		namer->txt_rect->h = 0;

		return(true);
	}

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					namer->name, GOLD);

	if (!surf)
	{
		set_errma(TTF_ER);
		return(false);
	}

	namer->txt_txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);


	if (!namer->txt_txr)
	{
		set_errma(SDL_ER);
		return(false);
	}


	if (SDL_QueryTexture(	namer->txt_txr, NULL, NULL,
				&namer->txt_rect->w, &namer->txt_rect->h) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}


	namer->txt_rect->x = 	namer->rect->x + (namer->rect->w / 2) -
				namer->txt_rect->w / 2;
	namer->txt_rect->y = 	namer->rect->y + (namer->rect->h / 3) -
				namer->txt_rect->h / 2;

	return(true);
}

void		del_namer(obj_t *namer)
{
	if (namer)
	{
		if (namer->obj)
		{
			if (((namer_t*)(namer->obj))->txt_txr)
			{
				SDL_DestroyTexture(
					((namer_t*)(namer->obj))->txt_txr);
				((namer_t*)(namer->obj))->txt_txr = NULL;
			}
			if (((namer_t*)(namer->obj))->txt_rect)
			{
				free(((namer_t*)(namer->obj))->txt_rect);
				((namer_t*)(namer->obj))->txt_rect = NULL;
			}

			// free these create a invalid read
			// that crash the game
			// if (((namer_t*)(namer->obj))->ok_but)
			// {
			// 	del_button(((namer_t*)(namer->obj))->ok_but);
			// 	((namer_t*)(namer->obj))->ok_but = NULL;
			// }
			// if (((namer_t*)(namer->obj))->back_but)
			// {
			// 	del_button(((namer_t*)(namer->obj))->back_but);
			// 	((namer_t*)(namer->obj))->back_but = NULL;
			// }

			if (((namer_t*)(namer->obj))->name)
			{
				free(((namer_t*)(namer->obj))->name);
				((namer_t*)(namer->obj))->name = NULL;
			}
			free(namer->obj);
			namer->obj = NULL;
		}
	}
}
