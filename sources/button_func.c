#include "world.h"
#include "button_func.h"
#include "button.h"
#include "scene.h"
#include "errma.h"
#include "pawn_picker.h"
#include "namer.h"
#include "map_chooser.h"

bool	load_difficulty_scene(world_t *world)
{
	if (world->current_scene->type == MAIN_MENU_S)
	{
		world->type	= NEW_CAMPAIGN;
		world->level	= LEVEL_1;
	}
	else
	{
		printf("ttoot\n");
		set_world_map(world);
		world->type = PLY_VS_IA;
	}

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					DIFFICULTY_S);
	return(false);
}
bool	continue_campaign(world_t *world)
{
	FILE	*file;

	if (!(file = fopen("autosave", "r")))
	{
		return(true);
	}

	if (fclose(file) == EOF)
	{
		perror("error, couldn't close autosave");
		return(true);
	}

	world->type = LOAD_CAMPAIGN;

	del_scene(world->current_scene);
	world->running = new_scene(world, world->current_scene, GAME_S);

	world->dif	= NO_DIF;
	world->type	= NO_TYPE;
	world->level	= NO_LV;

	return(false);
}
bool	set_diff_easy(world_t *world)
{
	world->dif = EASY_DIF;

	del_scene(world->current_scene);
	world->running = new_scene(world, world->current_scene, GAME_S);

	world->dif	= NO_DIF;
	world->type	= NO_TYPE;
	world->level	= NO_LV;

	return(false);
}
bool	set_diff_medium(world_t *world)
{
	world->dif = MED_DIF;

	del_scene(world->current_scene);
	world->running = new_scene(world, world->current_scene, GAME_S);

	world->dif	= NO_DIF;
	world->type	= NO_TYPE;
	world->level	= NO_LV;

	return(false);

}
bool	set_diff_hard(world_t *world)
{
	world->dif = HARD_DIF;

	del_scene(world->current_scene);
	world->running = new_scene(world, world->current_scene, GAME_S);

	world->dif	= NO_DIF;
	world->type	= NO_TYPE;
	world->level	= NO_LV;

	world->dif = NO_DIF;
	return(false);

}

bool	load_quick_game_menu_scene(world_t *world)
{
	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					QUICK_GAME_MENU_S);
	return(false);
}

bool	vs_ai(world_t *world)
{
	world->type = PLY_VS_IA;

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					MAP_CHOICE_S);

	return(false);
}

bool	vs_player_offline(world_t *world)
{
	world->type = VS_PLY_OFF;

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					MAP_CHOICE_S);

	return(false);
}

bool	vs_player_online(world_t *world)
{
	return(false);
}

bool	load_rules_scene(world_t *world)
{
	return(false);
}

bool	start_tutorial(world_t *world)
{
	return(false);
}

bool	load_main_menu_scene(world_t *world)
{
	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					MAIN_MENU_S);
	return(false);
}

bool	quit_game(world_t *world)
{
	world->running = false;
	return(false);
}

bool	load_settings_scene(world_t *world)
{
	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
						SETTINGS_S);
	return(false);
}

bool	dummy_function(world_t *world)
{
	return(true);
}

bool	load_credit_scene(world_t *world)
{
	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					CREDIT_S);
	return(false);
}

bool	option_fullscreen(world_t * world)
{
	if (SDL_SetWindowFullscreen(world->window,
		!(world->fullscreen) ? SDL_WINDOW_FULLSCREEN : 0) < 0)
	{
		set_errma(SDL_ER);
		world->running = false;
	}
	world->fullscreen = !(world->fullscreen);
	return(true);
}

bool	option_screen_size(world_t * world)
{
	// check next valide size
	// delete world->window world->renderer
	// modify world->window world->renderer
	return(true);
}

bool	option_lang(world_t * world)
{
	if ((world->lang += 1) == END_LANG)
	{
		world->lang = 0;
	}
	del_scene(world->current_scene);
	world->running = set_settings_menu(	world, world->current_scene,
						SETTINGS_S);
	return(false);
}

bool	save_edit(world_t * world)
{
	pawn_picker_t	*pwp;
	obj_t		*tmp;

	pwp = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (pwp->pawn_count[i][j] != 0)
			{
				return(true);
			}
		}

	}

	if (!(tmp = new_namer(world)))
	{
		return(true);
	}

	world->current_scene->elems[4] = world->current_scene->elems[3];
	world->current_scene->elems[3] = world->current_scene->elems[2];
	world->current_scene->elems[2] = world->current_scene->elems[1];
	world->current_scene->elems[1] = world->current_scene->elems[0];
	world->current_scene->elems[0] = tmp;

	return(false);
}

bool	pawn_picker_pharaoh(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 0) ? -1 : 0;
	}

	return(true);
}

bool	pawn_picker_scarab(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 1) ? -1 : 1;
	}

	return(true);
}

bool	pawn_picker_anubis(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 2) ? -1 : 2;
	}

	return(true);
}

bool	pawn_picker_pyramid(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 3) ? -1 : 3;
	}

	return(true);
}

bool	pawn_picker_sphinx(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 4) ? -1 : 4;
	}

	return(true);
}

bool	pawn_picker_red_square(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}


	if (pwp)
	{
		pwp->col = RED_PAWN;
		set_pawn_but_color(world, pwp);
	}

	return(true);
}

bool	pawn_picker_blue_square(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->col = BLUE_PAWN;
		set_pawn_but_color(world, pwp);
	}

	return(true);
}

bool	pawn_picker_r_arrow(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 7) ? -1 : 7;
	}

	return(true);
}

bool	pawn_picker_l_arrow(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 8) ? -1 : 8;
	}

	return(true);
}

bool	pawn_picker_delete(world_t *world)
{
	pawn_picker_t *pwp;

	pwp = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (pwp)
	{
		pwp->status = (pwp->status == 9) ? -1 : 9;
	}

	return(true);
}

bool	load_editor_scene(world_t *world)
{
	world->dif	= NO_DIF;
	world->type	= EDIT_MAP;
	world->level	= NO_LV;

	del_scene(world->current_scene);
	world->running = set_map_edit_scene(	world, world->current_scene,
						GAME_S);
	world->type	= NO_TYPE;

	return(false);
}

bool	namer_ok(world_t * world)
{
	board_t *brd;
	namer_t *nmr;

	brd = NULL;

	for (int i = 0; i < world->current_scene->elems[i]; i++)
	{

		if (world->current_scene->elems[i]->obj_type == BOARD_T)
		{
			brd = world->current_scene->elems[i]->obj;
		}
		if (world->current_scene->elems[i]->obj_type == NAMER_T)
		{
			nmr = world->current_scene->elems[i]->obj;
		}
	}

	if (!brd || !nmr)
	{
		fprintf(stderr,
			"Warning : Invalid placement of \"OK\" button\n");
		return(true);
	}

	if (!save_map(brd, nmr->name))
	{
		return(namer_back(world));
	}

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					MAIN_MENU_S);

	return(false);
}

bool	namer_back(world_t * world)
{
	del_namer(world->current_scene->elems[0]);

	world->current_scene->elems[0] = world->current_scene->elems[1];
	world->current_scene->elems[1] = world->current_scene->elems[2];
	world->current_scene->elems[2] = world->current_scene->elems[3];
	world->current_scene->elems[3] = world->current_scene->elems[4];
	world->current_scene->elems[4] = NULL;

	return(true);
}

bool	load_end_game_scene(world_t *world)
{
	del_scene(world->current_scene);
	world->running = set_end_scene(world, world->current_scene, END_S);

	return(false);
}

bool			up_map_chooser(world_t * world)
{
	map_chooser_t	*tmp;

	tmp = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == MAP_CHOOSER_T)
		{
			tmp = world->current_scene->elems[i]->obj;
		}
	}

	if (tmp)
	{
		tmp += ((tmp->top_map == 0) ? 0 : -1);
	}

	return(true);
}

bool	down_map_chooser(world_t * world)
{
	map_chooser_t	*tmp;

	tmp = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == MAP_CHOOSER_T)
		{
			tmp = world->current_scene->elems[i]->obj;
		}
	}

	if (tmp)
	{
		tmp += ((tmp->top_map == tmp->map_nb - 1) ? 0 : 1);
	}

	return(true);
}

bool	set_world_map(world_t *world)
{
	map_chooser_t	*tmp;

	tmp = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == MAP_CHOOSER_T)
		{
			tmp = world->current_scene->elems[i]->obj;
		}
	}

	if (tmp)
	{
		if (	(tmp->target < 0 || tmp->target >= tmp->map_nb) ||
			!tmp->maps_name || !tmp->maps_name[tmp->target])
		{
			return(true);
		}
		if (world->map)
		{
			free(world->map);
			world->map = NULL;
		}

		world->map = malloc((strlen(tmp->maps_name[tmp->target]) + 1)
					* sizeof(char));
		if (!world->map)
		{
			return(false);
		}
		memset(world->map, 0, (strlen(tmp->maps_name[tmp->target] + 1)));
		strcat(world->map, tmp->maps_name[tmp->target]);

		return(true);
	}

	return(false);
}

bool	load_next_level(world_t *world)
{
	switch (world->level)
	{
		case LEVEL_1:
			world->level = LEVEL_2;
			break;
		case LEVEL_2:
			world->level = LEVEL_3;
			break;
		case LEVEL_3:
		default:
			del_scene(world->current_scene);
			world->running = new_scene(	world,
							world->current_scene,
							MAIN_MENU_S);
			return(false);
	}
	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					GAME_S);

	return(false);
}

bool	save_and_back(world_t *world)
{
	board_t	*brd;

	brd = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == BOARD_T)
		{
			brd = world->current_scene->elems[i]->obj;
		}
	}

	if (brd)
	{
		printf("saving ...\n");
		save_game(brd);
	}
	else
	{
		printf("error no board\n");
		return(true);
	}

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					MAIN_MENU_S);

	return(false);
}

bool	pawn_rotation_r(world_t *world)
{
	board_t	*brd;

	brd = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == BOARD_T)
		{
			brd = world->current_scene->elems[i]->obj;
		}
	}

	if (brd)
	{
		TARGET_PAWN(brd)->orient = NORM(TARGET_PAWN(brd)->orient - 1);

		set_pawn_txr(world, TARGET_PAWN(brd));

		brd->in_anime = true;

		set_but_pos(brd);
	}

	return(true);
}

bool	pawn_rotation_l(world_t *world)
{
	board_t	*brd;

	brd = NULL;

	for (int i = 0; world->current_scene->elems[i]; i++)
	{
		if (world->current_scene->elems[i]->obj_type == BOARD_T)
		{
			brd = world->current_scene->elems[i]->obj;
		}
	}

	if (brd)
	{
		TARGET_PAWN(brd)->orient = NORM(TARGET_PAWN(brd)->orient + 1);

		set_pawn_txr(world, TARGET_PAWN(brd));

		brd->in_anime = true;
	}

	return(true);
}

bool	start_game(world_t *world)
{
	set_world_map(world);

	world->type = PLY_VS_PLY_LOCAL;

	del_scene(world->current_scene);
	world->running = new_scene(	world, world->current_scene,
					GAME_S);

	world->type = NO_TYPE;

	return(false);
}
