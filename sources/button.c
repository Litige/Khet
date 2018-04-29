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

	if (	new_but->txr == NULL ||
		new_but->rect == NULL ||
		new_but->info == NULL ||
		new_but->button_func == NULL ||
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

void			set_button_txr(	world_t 	*world,
					button_t	*but,
					const char	*info)
{
	SDL_Surface *surf;

	switch (but->txr_type)
	{
		case TXR_TEXT:
			surf = TTF_RenderUTF8_Blended(	world->ttf_font,
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


void			del_button(button_t *but)
{
	if (but->rect)
	{
		free(but->rect);
		but->rect = NULL;
	}
	if (but->txr)
	{
		SDL_DestroyTexture(but->txr);
		but->txr = NULL;
	}
	but->button_func = NULL;
	but->over_in_func = NULL;
	but->over_out_func = NULL;
	but->info = NULL;
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

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					but->info,
					WHITE);
	SDL_DestroyTexture(but->txr);
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

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					but->info,
					GOLD);
	SDL_DestroyTexture(but->txr);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	SDL_FreeSurface(surf);
}

void			set_button_func(button_t *but)
{
	switch (but->but_type)
	{
		case NEW_CAMP :
			but->button_func = &load_difficulty_scene;
			break;
		case CONT_CAMP :
			but->button_func = &continue_campaign;
			break;
		case EASY_DIF :
			but->button_func = &set_diff_easy;
			break;
		case MED_DIF :
			but->button_func = &set_diff_medium;
			break;
		case HARD_DIF :
			but->button_func = &set_diff_hard;
			break;
		case BACK :
			but->button_func = &load_main_menu_scene;
			break;
		case QUICK_GAME :
			but->button_func = &load_quick_game_menu_scene;
			break;
		case VS_AI :
			but->button_func = &vs_ai;
			break;
		case VS_PLY_ON :
			but->button_func = &vs_player_online;
			break;
		case VS_PLY_OFF :
			but->button_func = &vs_player_offline;
			break;
		case RULES :
			but->button_func = &load_rules_scene;
			break;
		case TUTO :
			but->button_func = &start_tutorial;
			break;
		case SETTINGS :
			but->button_func = &load_settings_scene;
			break;
		case EXIT :
			but->button_func = &quit_game;
			break;
		case OPT_FULLSCREEN :
			but->button_func = &option_fullscreen;
			break;
		case OPT_SCREEN_SIZE :
			but->button_func = &option_screen_size;
			break;
		case OPT_LANG :
			but->button_func = &option_lang;
			break;
		default :
			but->button_func = &dummy_function;
			break;
	}

}
