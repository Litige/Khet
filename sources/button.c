#include <stdint.h>	//for uint8_t
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "world.h"
#include "button.h"
#include "errma.h"
#include "colors.h"
#include "button_func.h"


button_t		*new_button(	world_t		*world,
					button_type_t	but_type,
					texture_type_t	txr_type,
					const char	*info,
					int		x,
					int		y,
					pos_type_t	ref)
{
	button_t	*new_but;

	if (	(new_but  = malloc(sizeof(button_t)))      == NULL ||
		(new_but->rect = malloc(sizeof(SDL_Rect))) == NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_but->txr_type	= txr_type;
	new_but->but_type	= but_type;
	new_but->obj_type	= BUTTON_T;
	new_but->overed		= false;
	new_but->info		= info;

	set_button_txr(world, new_but, info);
	set_button_rect(new_but, x, y, ref);
	set_over_func(new_but);
	set_button_func(new_but);

	new_but->button_func	= NULL;

	printf("new_but->txr (addr)           : %p\n", new_but->txr);
	printf("new_but->rect (addr)          : %p\n", new_but->rect);
	printf("new_but->info (addr)          : %p\n", new_but->info);
	printf("new_but->button_func (addr)   : %p\n", new_but->button_func);
	printf("new_but->over_in_func (addr)  : %p\n", new_but->over_in_func);
	printf("new_but->over_out_func (addr) : %p\n", new_but->over_out_func);

	if (	new_but->txr == NULL ||
		new_but->rect == NULL ||
		new_but->info == NULL ||
		new_but->over_in_func == NULL ||
		new_but->over_out_func == NULL)
	{
		return(NULL);
	}

	return(new_but);
}

void			set_button_rect(button_t	*but,
					int		x,
					int		y,
					pos_type_t	ref)
{
	// printf("start setting rect\n");
	if (SDL_QueryTexture(	but->txr, NULL, NULL,
				&but->rect->w, &but->rect->h) < 0)
	{
		set_errma(SDL_ER);
		return;
	}
	// printf("done with SDL_QueryTexture\n");

	switch (ref)
	{
		case CENTER:
			but->rect->x = x - but->rect->w / 2;
			but->rect->y = y - but->rect->h / 2;
			break;
		case TOPLEFT:
		default :
			but->rect->x = x;
			but->rect->y = y;
			break;
	}

}

void			set_button_txr(	world_t 	*world,
					button_t	*but,
					const char	*info)
{
	SDL_Surface *surf;

	switch (but->txr_type)
	{
		case TXR_TEXT:
			surf = TTF_RenderText_Blended(	world->ttf_font,
							info,
							GOLD);
			if (surf == NULL)
			{
				set_errma(SDL_ER);
				but->txr = NULL;
				break;
			}
			but->txr = SDL_CreateTextureFromSurface(world->renderer,
				 				surf);
			if (but->txr == NULL)
			{
				set_errma(SDL_ER);
			}
			SDL_FreeSurface(surf);
			break;
		case TXR_IMAGE:
			// load image into texture
			break;
		default:
			break;
	}
}


void			destroy_button(button_t *but)
{
	if (but->rect)
	{
		free(but->rect);
		but->rect = NULL;
	}
	if (but->button_func)
	{
		free(but->button_func);
		but->button_func = NULL;
	}
	if (but->over_in_func)
	{
		free(but->over_in_func);
		but->over_in_func = NULL;
	}
	if (but->over_out_func)
	{
		free(but->over_out_func);
		but->over_out_func = NULL;
	}
	if (but->txr)
	{
		SDL_DestroyTexture(but->txr);
		but->txr = NULL;
	}
	if (but->rect)
	{
		free(but->rect);
		but->rect = NULL;
	}
	if (but)
	{
		free(but);
		but = NULL;
	}
}

void			set_over_func(button_t *but)
{
	switch (but->txr_type)
	{
		case TXR_TEXT:
			but->over_in_func	= &over_in_txr_txt;
			but->over_out_func	= &over_out_txr_txt;
			break;
		case TXR_IMAGE:
			but->over_in_func	= &over_in_txr_image;
			but->over_out_func	= &over_out_txr_image;
			break;
		default:
			break;
	}
}


void			over_in_txr_image(world_t* world, button_t* but)
{
	// load img
	// whiten the image
}

void			over_in_txr_txt(world_t* world, button_t* but)
{
	SDL_Surface	*surf;

	surf = TTF_RenderText_Blended(	world->ttf_font,
					but->info,
					WHITE);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	SDL_FreeSurface(surf);
}

void			over_out_txr_image(world_t* world, button_t* but)
{
	// load img
}

void			over_out_txr_txt(world_t* world, button_t* but)
{
	SDL_Surface	*surf;

	surf = TTF_RenderText_Blended(	world->ttf_font,
					but->info,
					GOLD);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	SDL_FreeSurface(surf);
}

void			set_button_func(button_t *but)
{
	switch (but->but_type)
	{
		case NEW_CAMP :
			// but->button_func =
			break;
		case CONT_CAMP :
			// but->button_func =
			break;
		case EASY_DIF :
			// but->button_func =
			break;
		case MED_DIF :
			// but->button_func =
			break;
		case HARD_DIF :
			// but->button_func =
			break;
		case BACK :
			// but->button_func =
			break;
		case QUICK_GAME :
			// but->button_func =
			break;
		case VS_AI :
			// but->button_func =
			break;
		case VS_PLY_ON :
			// but->button_func =
			break;
		case VS_PLY_OFF :
			// but->button_func =
			break;
		case RULES :
			// but->button_func =
			break;
		case TUTO :
			// but->button_func =
			break;
		case EXIT :
			// but->button_func =
			break;
		default :
			but->button_func = &dummy_function;
			break;
	}

}
