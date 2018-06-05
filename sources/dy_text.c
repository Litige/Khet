
#include "world.h"
#include "object.h"
#include "dy_text.h"
#include "errma.h"
#include "colors.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

obj_t	*new_dy_text(	world_t		*world,
			const char	*info,
			void		*data,
			data_type_t	data_type,
			int		x,
			int		y,
			pos_type_t	ref)
{
	dy_text_t	*new_dyt;
	obj_t		*new_obj;

	if (!data || !info || (data_type < 0 || data_type > PLAYER))
	{
		return(NULL);
	}

	if (	(new_dyt 	= malloc(sizeof(dy_text_t))) 	== NULL ||
		(new_dyt->rect	= malloc(sizeof(SDL_Rect))) 	== NULL ||
		(new_obj	= malloc(sizeof(obj_t)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_dyt->data		= data;
	new_dyt->data_type	= data_type;
	new_dyt->info		= info;
	new_dyt->value		= NULL;

	new_obj->obj		= new_dyt;
	new_obj->obj_type	= DY_TEXT_T;


	if (!set_dy_text_txr(world, new_dyt) || !set_dy_text_rect(new_dyt))
	{
		del_dy_text(new_obj);
		return(NULL);
	}

	switch (ref)
	{
		case CENTER:
			new_dyt->rect->x = x - (new_dyt->rect->w / 2);
			new_dyt->rect->y = y - (new_dyt->rect->h / 2);
			break;
		case TOPLEFT:
		default :
			new_dyt->rect->x = x;
			new_dyt->rect->y = y;
			break;
	}

	return(new_obj);
}

bool	set_dy_text_rect(dy_text_t *text)
{
	if (SDL_QueryTexture(	text->txr, NULL, NULL,
				&text->rect->w, &text->rect->h) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	return(true);
}

bool			set_dy_text_txr(world_t *world, dy_text_t *text)
{
	char		*full_text;
	char		*new_value;
	SDL_Surface	*surf;

	if (!set_text_value(world, &full_text, &new_value, text))
	{
		return(false);
	}


	if(text->value && strcmp(new_value, text->value) == 0)
	{
		if (new_value)
		{
			free(new_value);
		}
		if (full_text)
		{
			free(full_text);
		}
		return(true);
	}

	sprintf(full_text, "%s : %s\0", text->info, new_value);

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					full_text,
					GOLD);
	if (surf == NULL)
	{
		set_errma(TTF_ER);
		text->txr = NULL;
		return(false);
	}

	text->txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);

	if (text->txr == NULL)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (text->value)
	{
		free(text->value);
	}
	text->value = new_value;
	if (full_text)
	{
		free(full_text);
	}

	return(true);
}

bool	set_text_value(	world_t		*world,
			char		**full_text,
			char		**new_value,
			dy_text_t	*text)
{
	switch (text->data_type)
	{
		case STRING :
			// full_text = malloc(	(strlen(text->info) +
			// 			strlen(text->data) + 1) *
			// 			sizeof(char));
			// new_value = malloc(	(strlen(text->data) + 1) *
			// 			sizeof(char));
			*full_text = NULL;
			*new_value = NULL;
			break;
		case CHAR :
			// *full_text = malloc(	(strlen(text->info) +
			// 			1 + 1) *
			// 			sizeof(char));
			// *new_value = malloc((1 + 1) * sizeof(char));
			*full_text = NULL;
			*new_value = NULL;
			break;
		case INT :
			*full_text = malloc(	(strlen(text->info) +
						10 + 4 + 1) *
						sizeof(char));
			memset(*full_text, 0, 	(strlen(text->info) +
						10 + 4 + 1) *
						sizeof(char));
			*new_value = malloc((10 + 1) * sizeof(char));
			memset(*new_value, 0, (10 + 1) * sizeof(char));
			break;
		case PLAYER :
			*full_text = malloc(	(strlen(text->info) +
						5 + 4 + 1) *
						sizeof(char));
			memset(*full_text, 0, 	(strlen(text->info) +
						5 + 4 + 1) *
						sizeof(char));
			*new_value = malloc((5 + 1) * sizeof(char));
			memset(*new_value, 0, (5 + 1) * sizeof(char));
			break;
		default :
			break;
	}

	if (!(*full_text) || !(*new_value))
	{
		set_errma(MALLOC);
		return(false);
	}

	switch (text->data_type)
	{
		case STRING :
			sprintf(*new_value, "%s\0",
				text->data);
			break;
		case CHAR :
			sprintf(*new_value, "%c\0",
				*((char*)text->data));
			break;
		case INT :
			sprintf(*new_value, "%i\0",
				*((int*)text->data));
			break;
		case PLAYER :
			sprintf(*new_value, "%s\0",
				TEXT[world->lang]
				[*((int*)text->data) == RED_PLY ? DY_RED :
								  DY_BLUE]);
			break;
		default :
			break;
	}

	return(true);
}

void	del_dy_text(obj_t *text)
{
	if (text)
	{
		if (text->obj)
		{
			if (((dy_text_t*)(text->obj))->rect)
			{
				free(((dy_text_t*)(text->obj))->rect);
				((dy_text_t*)(text->obj))->rect = NULL;
			}
			if (((dy_text_t*)(text->obj))->txr)
			{
				SDL_DestroyTexture(((dy_text_t*)(text->obj))
							->txr);
				((dy_text_t*)(text->obj))->txr = NULL;
			}
			if (((dy_text_t*)(text->obj))->value)
			{
				free(((dy_text_t*)(text->obj))->value);
				((dy_text_t*)(text->obj))->value = NULL;
			}
			((dy_text_t*)(text->obj))->data = NULL;
			((dy_text_t*)(text->obj))->info = NULL;
			free(text->obj);
			text->obj = NULL;
		}
		free(text);
		text = NULL;
	}
}
