#include "world.h"
#include "button_func.h"
#include "button.h"
#include "scene.h"
#include "errma.h"


bool	load_difficulty_scene(world_t *world)
{
	return(true);
}
bool	continue_campaign(world_t *world)
{
	return(true);
}
bool	set_diff_easy(world_t *world)
{
	return(true);
}
bool	set_diff_medium(world_t *world)
{
	return(true);
}
bool	set_diff_hard(world_t *world)
{

	return(true);
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

	return(true);
}

bool	vs_player_offline(world_t *world)
{

	return(true);
}

bool	vs_player_online(world_t *world)
{

	return(true);
}

bool	load_rules_scene(world_t *world)
{

}
bool	start_tutorial(world_t *world)
{

	return(true);
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
	return(true);
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

	return(true);
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
