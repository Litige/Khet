#include "scene.h"
#include "lang.h"
#include "button.h"
#include "errma.h"
#include "text.h"
#include "text_obj.h"
#include "object.h"
#include "images.h"
#include "pawn_picker.h"

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

void	del_scene(scene_t* scene)
{
	if (scene)
	{
		if (scene->elems)
		{
			for (int i = 0; scene->elems[i]; i++)
			{
				switch (scene->elems[i]->obj_type)
				{
					case BUTTON_T:
						del_button(scene->elems[i]);
						break;
					case TEXT_T:
						del_text(scene->elems[i]);
						break;
					case BOARD_T:
						delete_board(scene->elems[i]);
						break;
					case PAWN_PICKER_T:
						delete_pawn_picker(scene
								->elems[i]);
						break;
					case NAMER_T:
						del_namer(scene->elems[i]);
						break;
					default:
					fprintf(stderr,
					"Warning : unrecognized object type\n");
					break;
				}
			}
			free(scene->elems);
			scene->elems = NULL;
		}
	}
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

	if ((scene->elems = malloc(sizeof(obj_t*) * 10)) == NULL)
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
					MAP_EDITOR,
					TXR_TEXT,
					TEXT[world->lang][B_MAP_EDITOR],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 3,
					CENTER);
	scene->elems[4] = new_button(	world,
					RULES,
					TXR_TEXT,
					TEXT[world->lang][B_RULES],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 4,
					CENTER);
	scene->elems[5] = new_button(	world,
					SETTINGS,
					TXR_TEXT,
					TEXT[world->lang][B_SETTINGS],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 5,
					CENTER);
	scene->elems[6] = new_button(	world,
					CREDIT,
					TXR_TEXT,
					TEXT[world->lang][B_CREDITS],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 6,
					CENTER);
	scene->elems[7] = new_button(	world,
					EXIT,
					TXR_TEXT,
					TEXT[world->lang][B_EXIT],
					center_x,
					world->size[0] * 2 / 5 + MED_FONT * 7,
					CENTER);
	// add BG obj (in first pos)
	if (!set_font_size(world, BIG_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[8] = new_text(	world,
					TEXT[world->lang][T_MAIN_MENU],
					center_x, BIG_FONT, CENTER);
	if (!set_font_size(world, MED_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[9] = NULL;


	for (int i = 0 ; i < 9 ; i++)
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

	// add BG obj (in first pos)
	if (!set_font_size(world, BIG_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[4] = new_text(	world,
					TEXT[world->lang][T_SETTING_MENU],
					center_x, BIG_FONT, CENTER);
	if (!set_font_size(world, MED_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[5] = NULL;

	for (int i = 0 ; i < 5 ; i++)
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
	int	center_x = world->size[1] / 2;
	int	center_y = world->size[0] / 2;

	if ((scene->elems = malloc(sizeof(obj_t *) * 6)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}
	scene->type = type;

	if (!set_font_size(world, BIG_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[0] = new_text(	world,
					TEXT[world->lang][T_DIF_CHOICE],
					center_x, BIG_FONT, CENTER);
	if (!set_font_size(world, MED_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[1] = new_button(	world,
					MED_DIF,
					TXR_IMAGE,
					MED_DIF_BUT,
					center_x, center_y, CENTER);
	scene->elems[2] = new_button(
			world,
			EASY_DIF,
			TXR_IMAGE,
			EASY_DIF_BUT,
			center_x - 50 -
			((button_t*)(scene->elems[1]->obj))->rect->w,
			center_y, CENTER);
	scene->elems[3] = new_button(
			world,
			HARD_DIF,
			TXR_IMAGE,
			HARD_DIF_BUT,
			center_x + 50 +
			((button_t*)(scene->elems[1]->obj))->rect->w,
			center_y, CENTER);
	scene->elems[4] = new_button(	world,
					BACK,
					TXR_TEXT,
					TEXT[world->lang][B_BACK],
					center_x, center_y + MED_FONT * 5,
					CENTER);
	scene->elems[5] = NULL;

	for (int i = 0 ; i < 5 ; i++)
	{
		if (scene->elems[i] == NULL)
		{
			del_scene(scene);
			return(false);
		}
	}

	return(true);
}

bool	set_quick_game_menu(world_t *world, scene_t *scene, scene_type_t type)
{
	int	center_x = world->size[1] / 2;
	int	center_y = world->size[0] / 2;

	if ((scene->elems = malloc(sizeof(obj_t *) * 6)) == NULL)
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
	if (!set_font_size(world, BIG_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[4] = new_text(	world,
					TEXT[world->lang][T_QUICK_GAME_MENU],
					center_x, BIG_FONT, CENTER);
	if (!set_font_size(world, MED_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[5] = NULL;

	for (int i = 0 ; i < 5 ; i++)
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
	int	center_x = world->size[1] / 2;
	int	center_y = world->size[0] / 2;

	if ((scene->elems = malloc(sizeof(obj_t *) * 8)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	if (!set_font_size(world, BIG_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[0] = new_text(	world,
					TEXT[world->lang][T_CREDIT_MENU],
					center_x, BIG_FONT, CENTER);
	if (!set_font_size(world, MED_FONT))
	{
		del_scene(scene);
		return(false);
	}
	scene->elems[1] = new_text(	world,
					TEXT[world->lang][T_CREDIT_L1],
					center_x, center_y - MED_FONT * 2,
					CENTER);
	scene->elems[2] = new_text(	world,
					TEXT[world->lang][T_CREDIT_L2],
					center_x, center_y - MED_FONT, CENTER);
	scene->elems[3] = new_text(	world,
					TEXT[world->lang][T_CREDIT_L3],
					center_x, center_y, CENTER);
	scene->elems[4] = new_text(	world,
					TEXT[world->lang][T_CREDIT_L4],
					center_x, center_y + MED_FONT, CENTER);
	scene->elems[5] = new_text(	world,
					TEXT[world->lang][T_CREDIT_L5],
					center_x, center_y + MED_FONT * 2,
					CENTER);
	scene->elems[6] = new_button(	world,
					BACK,
					TXR_TEXT,
					TEXT[world->lang][B_BACK],
					center_x, center_y + MED_FONT * 5,
					CENTER);
	scene->elems[7] = NULL;

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

bool	set_game_scene(world_t *world, scene_t *scene, scene_type_t type)
{
	char	map[8][10][4];

	if ((scene->elems = malloc(sizeof(obj_t *) * 5)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	if (!map_loading(world, map))
	{
		del_scene(scene);
		return(false);
	}

	scene->elems[0] = NULL;	// game board
	scene->elems[1] = NULL; // turn
	scene->elems[2] = NULL; // current_ply

	scene->elems[3] = NULL; // save button

	scene->elems[4] = NULL;
	// load map if NEW_CAMP
	// load next map if new_level
	// load save if PLY_VS_PLY
	// side info (interactive text) (void* info enum info_type)
	//	turn
	//	current_ply
	// save button on CAMPAIGN mode else nothing
}

bool			set_map_edit_scene(	world_t		*world,
						scene_t		*scene,
						scene_type_t	type)
{
	char		map[8][10][4];
	pawn_picker_t	*pwp;

	if ((scene->elems = malloc(sizeof(obj_t *) * 6)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}

	scene->type = type;

	empty_map(map);
	set_square_layout(map);

	scene->elems[0] = new_pawn_picker(world); // pawn picker
	scene->elems[1] = new_board(world, map);

	if (!scene->elems[0])
	{
		del_scene(scene);
		return(false);
	}

	pwp = scene->elems[0]->obj;

	scene->elems[2] = new_button(	world,
					SAVE_EDIT,
					TXR_TEXT,
					TEXT[world->lang][B_SAVE_EDIT],
					pwp->rect->x + (pwp->rect->w / 2),
					(pwp->rect->y + pwp->rect->h) +
					((world->size[0] -
					(pwp->rect->y + pwp->rect->h)) / 3),
					CENTER);
	scene->elems[3] = new_button(	world,
					BACK,
					TXR_TEXT,
					TEXT[world->lang][B_BACK],
					pwp->rect->x + (pwp->rect->w / 2),
					(pwp->rect->y + pwp->rect->h) +
					(((world->size[0] -
					(pwp->rect->y + pwp->rect->h)) / 3) * 2),
					CENTER);

	scene->elems[4] = NULL;	// for namer
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

bool	set_rules_scene(world_t *world, scene_t *scene, scene_type_t type)
{
	// screenshot of the game and text
	// button BACK
	// button next and previous rule
}
