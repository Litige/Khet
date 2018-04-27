#include "scene.h"
#include "lang.h"
#include "button.h"
#include "errma.h"
#include "text.h"

#include <stdbool.h>

// prototypes are declared here because of
// conflictes with world.h during linking

bool	new_scene(world_t *world, scene_t *scene, scene_type_t type);
bool	del_scene(scene_t *scene);
bool	set_main_menu(world_t *world, scene_t *scene, scene_type_t type);

bool	new_scene(world_t *world, scene_t *scene, scene_type_t type)
{
	switch (type)
	{
		case MAIN_MENU_S:
			printf("generating a main menu scene\n");
			return(set_main_menu(world, scene, type));
			break;
		case SETTINGS_S:

			break;
		case DIFFICULTY_S:

			break;
		case QUICK_GAME_MENU_S:

			break;
		case CREDIT_S:

			break;
		case GAME_S:

			break;
		case RULES_S:

			break;
		default :
			return(false);
			break;
	}
	return(true);
}

bool		set_main_menu(world_t *world, scene_t *scene, scene_type_t type)
{
	int	center_x = world->size[0] / 2;
	int	center_y = world->size[1] / 2;

	scene->elems = NULL;
	printf("malloc start\n");

	if ((scene->elems = malloc(sizeof(void *) * 9)) == NULL)
	{
		set_errma(MALLOC);
		return(false);
	}
	printf("malloc end\n");

	scene->type = type;

	printf("generating various elements\n");

	scene->elems[0] = new_button(	world,
					NEW_CAMP,
					TXR_TEXT,
					TEXT[world->lang][B_NEW_CAMP],
					center_x, world->size[1] / 5, CENTER);
	scene->elems[1] = new_button(	world,
					CONT_CAMP,
					TXR_TEXT,
					TEXT[world->lang][B_CONT_CAMP],
					center_x, world->size[1] / 5 + MED_FONT * 1, CENTER);
	scene->elems[2] = new_button(	world,
					QUICK_GAME,
					TXR_TEXT,
					TEXT[world->lang][B_QUICK_GAME],
					center_x, world->size[1] / 5 + MED_FONT * 2, CENTER);
	scene->elems[3] = new_button(	world,
					RULES,
					TXR_TEXT,
					TEXT[world->lang][B_RULES],
					center_x, world->size[1] / 5 + MED_FONT * 3, CENTER);
	scene->elems[4] = new_button(	world,
					SETTINGS,
					TXR_TEXT,
					TEXT[world->lang][B_SETTINGS],
					center_x, world->size[1] / 5 + MED_FONT * 4, CENTER);
	scene->elems[5] = new_button(	world,
					CREDIT,
					TXR_TEXT,
					TEXT[world->lang][B_CREDITS],
					center_x, world->size[1] / 5 + MED_FONT * 5, CENTER);
	scene->elems[6] = new_button(	world,
					EXIT,
					TXR_TEXT,
					TEXT[world->lang][B_EXIT],
					center_x, world->size[1] / 5 + MED_FONT * 6, CENTER);
	// add title Text obj
	// scene->elems[7] =
	scene->elems[7] = NULL;
	scene->elems[8] = NULL;

	for (int i = 0 ; i < 7 ; i++)
	{
		if (scene->elems[i] == NULL)
		{
			i -= 1;
			while (i > 0)
			{
				free(scene->elems[i]);
				i -= 1;
			}
			return(false);
		}
	}

	return(true);
}

bool	del_scene(scene_t* scene)
{

}
