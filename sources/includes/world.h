#pragma once

#include "scene.h"
#include "lang.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef _WIN32
	#define RES_P "ressources\\"
	#define MAP_P "map\\"
#else
	#define RES_P "ressources/"
	#define MAP_P "map/"
#endif

typedef enum	game_type_s game_type_t;
enum		game_type_s
{
	PLY_VS_PLY_LOCAL,
	PLY_VS_PLY_ONLINE,
	PLY_VS_IA,
	NEW_CAMPAIGN,
	LOAD_CAMPAIGN,
	EDIT_MAP,
	NO_TYPE
};

typedef enum 	difficulty_e difficulty_t;
enum 		difficulty_e
{
	EASY,
	MEDIUM,
	HARD,
	NO_DIF
};

typedef enum 	camp_level_e camp_level_t;
enum 		camp_level_e
{
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	NO_LV
};

typedef struct		world_s world_t;
struct 			world_s
{
	SDL_Window	*window;
	SDL_Renderer	*renderer;
	TTF_Font	*ttf_font;
	SDL_Event	*event_handler;

	scene_t		*current_scene;

	lang_t		lang;

	difficulty_t	dif;
	game_type_t	type;
	camp_level_t	level;

	bool		running;
	bool		fullscreen;

	bool		click;
	bool		enter;
	bool		bk_space;
	int		mouse_pos[2];
	char		txt_input[32];

	int		size[2];
};

bool		is_txt_input(world_t *world);
