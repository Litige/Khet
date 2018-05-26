#include "world.h"
#include "text_obj.h"
#include "errma.h"
#include "colors.h"
#include "object.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


obj_t		*new_text(	world_t		*world,
				const char	*info,
				int		x,
				int		y,
				pos_type_t	ref)
{
	text_t	*new_text;
	obj_t	*new_obj;

	if (	(new_text	= malloc(sizeof(text_t)))	== NULL ||
		(new_text->rect	= malloc(sizeof(SDL_Rect)))	== NULL ||
		(new_obj	= malloc(sizeof(obj_t)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_text->info		= info;


	set_text_txr(world, new_text, info);
	set_text_rect(new_text, x, y, ref);

	if (	new_text->txr == NULL ||
		new_text->rect == NULL ||
		new_text->info == NULL)
	{
		return(NULL);
	}

	new_obj->obj		= new_text;
	new_obj->obj_type	= TEXT_T;

	return(new_obj);
}

void			set_text_rect(	text_t	*but,
					int		x,
					int		y,
					pos_type_t	ref)
{
	if (SDL_QueryTexture(	but->txr, NULL, NULL,
				&but->rect->w, &but->rect->h) < 0)
	{
		set_errma(SDL_ER);
		return;
	}

	switch (ref)
	{
		case CENTER:
			but->rect->x = x - (but->rect->w / 2);
			but->rect->y = y - (but->rect->h / 2);
			break;
		case TOPLEFT:
		default :
			but->rect->x = x;
			but->rect->y = y;
			break;
	}

}

void			set_text_txr(	world_t 	*world,
					text_t		*but,
					const char	*info)
{
	SDL_Surface *surf;

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					info,
					GOLD);
	if (surf == NULL)
	{
		set_errma(SDL_ER);
		but->txr = NULL;
		return ;
	}
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
		 				surf);
	if (but->txr == NULL)
	{
		set_errma(SDL_ER);
	}

	SDL_FreeSurface(surf);
}

void			del_text(obj_t *text)
{
	if (text)
	{
		if (text->obj)
		{
			if (((text_t*)(text->obj))->rect)
			{
				free(((text_t*)(text->obj))->rect);
				((text_t*)(text->obj))->rect = NULL;
			}
			if (((text_t*)(text->obj))->txr)
			{
				SDL_DestroyTexture(((text_t*)(text->obj))->txr);
				((text_t*)(text->obj))->txr = NULL;
			}
			((text_t*)(text->obj))->info = NULL;
			free(text->obj);
			text->obj = NULL;
		}
		free(text);
		text = NULL;
	}
}
