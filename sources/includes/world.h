#pragma once

#include "scene.h"
#include "lang.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct		world_s world_t;
struct 			world_s
{
	scene_t		*current_scene;
	SDL_Window	*window;
	SDL_Renderer	*renderer;
	TTF_Font	*ttf_font;
	SDL_Event	*event_handler;
	lang_t		lang;
	bool		running;
	bool		click;
	bool		fullscreen;
	int		mouse_pos[2];
	int		size[2];
};

typedef enum 	obj_type_e obj_type_t;
enum 		obj_type_e
{
	BUTTON_T,
	TEXT_T,
	BOARD_T
};

typedef enum 	difficulty_e difficulty_t;
enum 		difficulty_e
{
	EASY,
	MEDIUM,
	HARD
};
