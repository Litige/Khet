#pragma once


#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum 	texture_type_s texture_type_t;
enum 		texture_type_s
{
	TXR_TEXT,
	TXR_IMAGE
};

typedef enum 	button_type_s button_type_t;
enum 		button_type_s
{
	NEW_CAMP,
	CONT_CAMP,
	EASY_DIF,
	MED_DIF,
	HARD_DIF,
	BACK,
	QUICK_GAME,
	VS_AI,
	VS_PLY_ON,
	VS_PLY_OFF,
	RULES,
	TUTO,
	SETTINGS,
	CREDIT,
	EXIT,
	MAP_EDITOR,
	SAVE_EDIT,
	SAVE,
	OPT_FULLSCREEN,
	OPT_SCREEN_SIZE,
	OPT_LANG,
	NEXT,
	START_GAME,
	NAMER_OK,
	NAMER_BACK,
	UP_MCH,
	DW_MCH,
	PP_PHARAOH,
	PP_SCARAB,
	PP_ANUBIS,
	PP_PYRAMID,
	PP_SPHINX,
	PP_RED_SQUARE,
	PP_BLUE_SQUARE,
	PP_R_ARROW,
	PP_L_ARROW,
	PP_DELETE,
	BRD_ROT_R,
	BRD_ROT_L
};

typedef struct			button_s button_t;

#include "world.h"

struct				button_s
{
	texture_type_t		txr_type;
	button_type_t		but_type;

	SDL_Texture		*txr;
	SDL_Rect		*rect;
	bool			overed;
	char			*info;

	bool			(*button_func)(world_t*);
	void			(*over_in_func)(world_t*, button_t*);
	void			(*over_out_func)(world_t*, button_t*);
};

obj_t		*new_button(	world_t		*world,
				button_type_t	but_type,
				texture_type_t	txr_type,
				const char	*info,
				int		x,
				int		y,
				pos_type_t	ref);
void		over_in_txr_txt(world_t* world, button_t* but);
void		over_in_txr_image(world_t* world, button_t* but);
void		over_out_txr_image(world_t* world, button_t* but);
void		over_out_txr_txt(world_t* world, button_t* but);
bool		set_button_rect(button_t	*but,
				int		x,
				int		y,
				pos_type_t	ref);
bool		set_button_txr(	world_t		*world,
				button_t	*but,
				const char	*info);
void		set_over_func(button_t *but);
void		set_button_func(button_t *but);
void		destroy_button(button_t *but);
