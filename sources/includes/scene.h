#pragma once

#include "object.h"

typedef enum	scene_type_s scene_type_t;
enum		scene_type_s
{
	MAIN_MENU_S,
	SETTINGS_S,
	DIFFICULTY_S,
	QUICK_GAME_MENU_S,
	CREDIT_S,
	GAME_S,
	RULES_S,
	END_S,
	MAP_CHOICE_S
};

typedef struct	scene_s scene_t;
struct		scene_s
{
	obj_t		**elems;
	scene_type_t	type;
};
