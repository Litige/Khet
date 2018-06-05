#include "button.h"
#include "world.h"
#include "errma.h"
#include "colors.h"
#include "button_func.h"
#include "object.h"
#include "images.h"

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_rotozoom.h>



obj_t		*new_button(	world_t		*world,
				button_type_t	but_type,
				texture_type_t	txr_type,
				const char	*info,
				int		x,
				int		y,
				pos_type_t	ref)
{
	button_t	*new_but;
	obj_t		*new_obj;

	if (	(new_but	= malloc(sizeof(button_t))) 	== NULL ||
		(new_but->rect	= malloc(sizeof(SDL_Rect))) 	== NULL ||
		(new_obj	= malloc(sizeof(obj_t)))	== NULL)
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_obj->obj		= new_but;
	new_obj->obj_type	= BUTTON_T;

	new_but->txr_type	= txr_type;
	new_but->but_type	= but_type;
	new_but->overed		= false;
	new_but->info		= info;

	if (	!set_button_txr(world, new_but, info) ||
		!set_button_rect(new_but, x, y, ref))
	{
		del_button(new_obj);
		return(NULL);
	}

	set_over_func(new_but);
	set_button_func(new_but);

	return(new_obj);
}

bool			set_button_rect(button_t	*but,
					int		x,
					int		y,
					pos_type_t	ref)
{
	if (SDL_QueryTexture(	but->txr, NULL, NULL,
				&but->rect->w, &but->rect->h) < 0)
	{
		set_errma(SDL_ER);
		return(false);
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

	return(true);

}

bool			set_button_txr(	world_t 	*world,
					button_t	*but,
					const char	*info)
{
	SDL_Surface *surf;

	switch (but->txr_type)
	{
		case TXR_TEXT:
			surf = TTF_RenderUTF8_Blended(	world->ttf_font,
							info, GOLD);
			break;
		case TXR_IMAGE:
			surf = IMG_Load(but->info);
			break;
		default:
			break;
	}


	if (!surf)
	{
		set_errma(but->txr_type == TXR_TEXT ? TTF_ER : IMG_ER);
		but->txr = NULL;
		return(false);
	}

	but->txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);

	if (!but->txr)
	{
		set_errma(SDL_ER);
		return(false);
	}


	return(true);
}


void			del_button(obj_t *but)
{
	if (but)
	{
		if (but->obj)
		{
			if (((button_t*)(but->obj))->rect)
			{
				free(((button_t*)(but->obj))->rect);
				((button_t*)(but->obj))->rect = NULL;
			}
			if (((button_t*)(but->obj))->txr)
			{
				SDL_DestroyTexture(((button_t*)(but->obj))->txr);
				((button_t*)(but->obj))->txr = NULL;
			}
			((button_t*)(but->obj))->button_func = NULL;
			((button_t*)(but->obj))->over_in_func = NULL;
			((button_t*)(but->obj))->over_out_func = NULL;
			((button_t*)(but->obj))->info = NULL;
			free(but->obj);
			but->obj = NULL;
		}
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
	SDL_Surface	*surface;
	Uint32 		*target_pixel;
	SDL_Color	new_color;


	surface = IMG_Load(but->info);

	if (!surface)
	{
		set_errma(SDL_ER);
		return(false);
	}

	if (SDL_LockSurface(surface) != 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	for (int y = 0; y < surface->w; y++)
	{
		for (int x = 0; x < surface->h; x++)
		{
			target_pixel = 	(Uint8 *) surface->pixels +
					y * surface->pitch +
					x * sizeof(*target_pixel);
			if (*target_pixel != *((Uint32*)&TRANPARENCY))
			{
				new_color	= *((SDL_Color *)target_pixel);
				new_color.a	= 128;
				*target_pixel	= *((Uint32 *)&new_color);
			}
		}
	}


	SDL_UnlockSurface(surface);


	SDL_DestroyTexture(but->txr);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surface);
	if (but->txr == NULL)
	{
		set_errma(SDL_ER);
	}
	SDL_FreeSurface(surface);
}

void			over_in_txr_txt(world_t* world, button_t* but)
{
	SDL_Surface	*surf;

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					but->info,
					WHITE);
	if (surf == NULL)
	{
		set_errma(but->txr_type == TXR_TEXT ? SDL_ER : IMG_ER);
		but->txr = NULL;
		return;
	}

	SDL_DestroyTexture(but->txr);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	if (but->txr == NULL)
	{
		set_errma(SDL_ER);
	}
	SDL_FreeSurface(surf);
}

void			over_out_txr_image(world_t* world, button_t* but)
{
	SDL_Surface	*surf;

	surf = IMG_Load(but->info);

	if (surf == NULL)
	{
		set_errma(but->txr_type == TXR_TEXT ? SDL_ER : IMG_ER);
		but->txr = NULL;
		return;
	}

	SDL_DestroyTexture(but->txr);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	if (but->txr == NULL)
	{
		set_errma(SDL_ER);
	}
	SDL_FreeSurface(surf);
}

void			over_out_txr_txt(world_t* world, button_t* but)
{
	SDL_Surface	*surf;

	surf = TTF_RenderUTF8_Blended(	world->ttf_font,
					but->info,
					GOLD);
	if (surf == NULL)
	{
		set_errma(but->txr_type == TXR_TEXT ? TTF_ER : IMG_ER);
		but->txr = NULL;
		return;
	}

	SDL_DestroyTexture(but->txr);
	but->txr = SDL_CreateTextureFromSurface(world->renderer,
						surf);
	SDL_FreeSurface(surf);
	if (but->txr == NULL)
	{
		set_errma(SDL_ER);
	}
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
		case CREDIT :
			but->button_func = &load_credit_scene;
			break;
		case EXIT :
			but->button_func = &quit_game;
			break;
		case MAP_EDITOR :
			but->button_func = &load_editor_scene;
			break;
		case SAVE_EDIT :
			but->button_func = &save_edit;
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
		case PP_PHARAOH :
			but->button_func = &pawn_picker_pharaoh;
			break;
		case PP_SCARAB :
			but->button_func = &pawn_picker_scarab;
			break;
		case PP_ANUBIS :
			but->button_func = &pawn_picker_anubis;
			break;
		case PP_PYRAMID :
			but->button_func = &pawn_picker_pyramid;
			break;
		case PP_SPHINX :
			but->button_func = &pawn_picker_sphinx;
			break;
		case PP_RED_SQUARE :
			but->button_func = &pawn_picker_red_square;
			break;
		case PP_BLUE_SQUARE :
			but->button_func = &pawn_picker_blue_square;
			break;
		case PP_R_ARROW :
			but->button_func = &pawn_picker_r_arrow;
			break;
		case PP_L_ARROW :
			but->button_func = &pawn_picker_l_arrow;
			break;
		case PP_DELETE :
			but->button_func = &pawn_picker_delete;
			break;
		case NAMER_OK :
			but->button_func = &namer_ok;
			break;
		case NAMER_BACK :
			but->button_func = &namer_back;
			break;
		case UP_MCH :
			but->button_func = &up_map_chooser;
			break;
		case DW_MCH :
			but->button_func = &down_map_chooser;
			break;
		case NEXT :
			but->button_func = &load_next_level;
			break;
		case START_GAME :
			but->button_func = &start_game;
			break;
		case SAVE :
			but->button_func = &save_and_back;
			break;
		case BRD_ROT_R :
			but->button_func = &pawn_rotation_r;
			break;
		case BRD_ROT_L :
			but->button_func = &pawn_rotation_l;
			break;
		default :
			but->button_func = &dummy_function;
			break;
	}

}
