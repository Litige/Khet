#include "scene.h"
#include "lang.h"
#include "button.h"
#include "errma.h"
#include "text.h"

#include <stdbool.h>

// prototypes are declared here because of
// conflictes with world.h during linking

bool	new_scene(world_t *world, scene_t *scene, scene_type_t type);
bool	set_main_menu(world_t *world, scene_t *scene, scene_type_t type);
bool	set_settings_menu(world_t *world, scene_t *scene, scene_type_t type);
bool	set_difficulty_menu(world_t *world, scene_t *scene, scene_type_t type);
bool	set_quick_game_menu(world_t *world, scene_t *scene, scene_type_t type);
bool	set_credit(world_t *world, scene_t *scene, scene_type_t type);
bool	set_game_scene(world_t *world, scene_t *scene, scene_type_t type);
bool	set_rules_scene(world_t *world, scene_t *scene, scene_type_t type);
void	del_scene(scene_t *scene);
void	del_main_menu_scene(scene_t *scene);
void	del_settings_scene(scene_t *scene);
void	del_difficulty_scene(scene_t *scene);
void	del_quick_game_menu_scene(scene_t *scene);
void	del_credit_scene(scene_t *scene);
void	del_game_scene(scene_t *scene);
void	del_rules_scene(scene_t *scene);

void	del_scene(scene_t* scene)
{
	if (scene->elems)
	{
		switch (scene->type)
		{
			case MAIN_MENU_S :
				del_main_menu_scene(scene);
				break;
			case SETTINGS_S :
				del_settings_scene(scene);
				break;
			case DIFFICULTY_S :
				del_difficulty_scene(scene);
				break;
			case QUICK_GAME_MENU_S :
				del_quick_game_menu_scene(scene);
				break;
			case CREDIT_S :
				del_credit_scene(scene);
				break;
			case GAME_S :
				del_game_scene(scene);
				break;
			case RULES_S :
				del_rules_scene(scene);
				break;
		}
		free(scene->elems);
	}
}

void	del_main_menu_scene(scene_t *scene)
{
	for (int i = 0; i < 7; i++)
	{
		if (scene->elems[i])
		{
			del_button(scene->elems[i]);
		}
	}
}

void	del_settings_scene(scene_t *scene)
{
	for (int i = 0; i < 4; i++)
	{
		if (scene->elems[i])
		{
			del_button(scene->elems[i]);
		}
	}
}

void	del_difficulty_scene(scene_t *scene)
{

}

void	del_quick_game_menu_scene(scene_t *scene)
{
	for (int i = 0; i < 4; i++)
	{
		if (scene->elems[i])
		{
			del_button(scene->elems[i]);
		}
	}
}

void	del_credit_scene(scene_t *scene)
{

}

void	del_game_scene(scene_t *scene)
{

}

void	del_rules_scene(scene_t *scene)
{

}

bool	new_scene(world_t *world, scene_t *scene, scene_type_t type)
{
	switch (type)
	{
		case MAIN_MENU_S:
			return(set_main_menu(world, scene, type));
			break;
		case SETTINGS_S:
			return(set_settings_menu(world, scene, type));
			break;
		case DIFFICULTY_S:
			return(set_difficulty_menu(world, scene, type));
			break;
		case QUICK_GAME_MENU_S:
			return(set_quick_game_menu(world, scene, type));
			break;
		case CREDIT_S:
			return(set_credit(world, scene, type));
			break;
		case GAME_S:
			return(set_game_scene(world, scene, type));
			break;
		case RULES_S:
			return(set_rules_scene(world, scene, type));
			break;
		default :
			return(false);
			break;
	}
	return(true);
}

bool	set_main_menu(world_t *world, scene_t *scene, scene_type_t type)
{
	int	center_x = world->size[1] / 2;

	if ((scene->elems = malloc(sizeof(void *) * 9)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	scene->elems[0] = new_button(	world,
					NEW_CAMP,
					TXR_TEXT,
					TEXT[world->lang][B_NEW_CAMP],
					center_x, world->size[0] * 2 / 5, CENTER);
	scene->elems[1] = new_button(	world,
					CONT_CAMP,
					TXR_TEXT,
					TEXT[world->lang][B_CONT_CAMP],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 1,
					CENTER);
	scene->elems[2] = new_button(	world,
					QUICK_GAME,
					TXR_TEXT,
					TEXT[world->lang][B_QUICK_GAME],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 2,
					CENTER);
	scene->elems[3] = new_button(	world,
					RULES,
					TXR_TEXT,
					TEXT[world->lang][B_RULES],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 3,
					CENTER);
	scene->elems[4] = new_button(	world,
					SETTINGS,
					TXR_TEXT,
					TEXT[world->lang][B_SETTINGS],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 4,
					CENTER);
	scene->elems[5] = new_button(	world,
					CREDIT,
					TXR_TEXT,
					TEXT[world->lang][B_CREDITS],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 5,
					CENTER);
	scene->elems[6] = new_button(	world,
					EXIT,
					TXR_TEXT,
					TEXT[world->lang][B_EXIT],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 6,
					CENTER);
	// add title Text obj
	// add BG obj (in first pos)
	// scene->elems[7] =
	scene->elems[7] = NULL;
	scene->elems[8] = NULL;

	for (int i = 0 ; i < 7 ; i++)
	{
		if (scene->elems[i] == NULL)
		{
			del_scene(scene);
			return(false);
		}
	}

	return(true);
}

bool	set_settings_menu(world_t *world, scene_t *scene, scene_type_t type)
{
	int	center_x = world->size[1] / 2;
	int	center_y = world->size[0] / 2;

	// printf("world->size[0] = %i\n", world->size[0]);
	// printf("world->size[1] = %i\n", world->size[1]);

	if ((scene->elems = malloc(sizeof(void *) * 6)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	scene->elems[0] = new_button(	world,
					OPT_FULLSCREEN,
					TXR_TEXT,
					TEXT[world->lang][B_OPT_FULLSCREEN],
					center_x, center_y - MED_FONT * 1,
					CENTER);
	scene->elems[1] = new_button(	world,
					OPT_SCREEN_SIZE,
					TXR_TEXT,
					TEXT[world->lang][B_OPT_SCREEN_SIZE],
					center_x, center_y,
					CENTER);
	scene->elems[2] = new_button(	world,
					OPT_LANG,
					TXR_TEXT,
					TEXT[world->lang][B_OPT_LANG],
					center_x, center_y + MED_FONT * 1,
					CENTER);
	scene->elems[3] = new_button(	world,
					BACK,
					TXR_TEXT,
					TEXT[world->lang][B_BACK],
					center_x, center_y + MED_FONT * 2,
					CENTER);

	// add title Text obj
	// add BG obj (in first pos)
	// scene->elems[7] =
	scene->elems[4] = NULL;
	scene->elems[5] = NULL;

	for (int i = 0 ; i < 4 ; i++)
	{
		if (scene->elems[i] == NULL)
		{
			del_scene(scene);
			return(false);
		}
	}

	return(true);
}

bool	set_difficulty_menu(world_t *world, scene_t *scene, scene_type_t type)
{
}

bool	set_quick_game_menu(world_t *world, scene_t *scene, scene_type_t type)
{
	int	center_x = world->size[1] / 2;
	int	center_y = world->size[0] / 2;

	if ((scene->elems = malloc(sizeof(void *) * 6)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	scene->elems[0] = new_button(	world,
					VS_AI,
					TXR_TEXT,
					TEXT[world->lang][B_VS_IA],
					center_x, center_y - MED_FONT * 1,
					CENTER);
	scene->elems[1] = new_button(	world,
					VS_PLY_ON,
					TXR_TEXT,
					TEXT[world->lang][B_VS_PL_ON],
					center_x, center_y, CENTER);
	scene->elems[2] = new_button(	world,
					VS_PLY_OFF,
					TXR_TEXT,
					TEXT[world->lang][B_VS_PL_OFF],
					center_x, center_y + MED_FONT * 1,
					CENTER);
	scene->elems[3] = new_button(	world,
					BACK,
					TXR_TEXT,
					TEXT[world->lang][B_BACK],
					center_x, center_y + MED_FONT * 2,
					CENTER);

	// add title Text obj
	// add BG obj (in first pos)
	// scene->elems[7] =
	scene->elems[4] = NULL;
	scene->elems[5] = NULL;

	for (int i = 0 ; i < 4 ; i++)
	{
		if (scene->elems[i] == NULL)
		{
			del_scene(scene);
			return(false);
		}
	}

	return(true);
}

bool	set_credit(world_t *world, scene_t *scene, scene_type_t type)
{
}

bool	set_game_scene(world_t *world, scene_t *scene, scene_type_t type)
{
}

bool	set_rules_scene(world_t *world, scene_t *scene, scene_type_t type)
{
}
