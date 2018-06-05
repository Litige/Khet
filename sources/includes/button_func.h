#pragma once

#include "world.h"

// NEW_CAMP button
bool	load_difficulty_scene(world_t *world);
// CONT_CAMP button
bool	continue_campaign(world_t *world);
// EASY_DIF button
bool	set_diff_easy(world_t *world);
// MED_DIF button
bool	set_diff_medium(world_t *world);
// HARD_DIF button
bool	set_diff_hard(world_t *world);

// QUICK_GAME button
bool	load_quick_game_menu_scene(world_t *world);
// VS_AI button
bool	vs_ai(world_t *world);
// VS_PLY_ON button
bool	vs_player_offline(world_t *world);
// VS_PLY_OFF button
bool	vs_player_online(world_t *world);

// RULES button
bool	load_rules_scene(world_t *world);
// TUTO button
bool	start_tutorial(world_t *world);

// BACK button
bool	load_main_menu_scene(world_t *world);
// EXIT button
bool	quit_game(world_t *world);
// SETTINGS button
bool	load_settings_scene(world_t *world);
// CREDIT button
bool	load_credit_scene(world_t *world);

// used for usseless button
bool	dummy_function(world_t *world);

// fullscreen option button
bool	option_fullscreen(world_t * world);
// screen size option button
bool	option_screen_size(world_t * world);
// lang option button
bool	option_lang(world_t * world);

// MAP_EDITOR button
bool	load_editor_scene(world_t *world);
// save an map in the editor
bool	save_edit(world_t * world);

// pawn picker pharaoh button
bool	pawn_picker_pharaoh(world_t * world);
// pawn picker scarab button
bool	pawn_picker_scarab(world_t * world);
// pawn picker anubis button
bool	pawn_picker_anubis(world_t * world);
// pawn picker pyramid button
bool	pawn_picker_pyramid(world_t * world);
// pawn picker sphinx button
bool	pawn_picker_sphinx(world_t * world);
// pawn picker red_square button
bool	pawn_picker_red_square(world_t * world);
// pawn picker blue_square button
bool	pawn_picker_blue_square(world_t * world);
// pawn picker r_arrow button
bool	pawn_picker_r_arrow(world_t * world);
// pawn picker l_arrow button
bool	pawn_picker_l_arrow(world_t * world);
// pawn picker delete button
bool	pawn_picker_delete(world_t * world);

// namer ok button
bool	namer_ok(world_t * world);
// namer back button
bool	namer_back(world_t * world);

// used when a game end not really attached to a button
bool	load_end_game_scene(world_t *world);

// map chooser up button
bool	up_map_chooser(world_t *world);

// map chooser up button
bool	down_map_chooser(world_t *world);

// load next level
bool	load_next_level(world_t *world);

// called by some button function
bool	set_world_map(world_t *world);

// generate autosave
bool	save_and_back(world_t *world);

// rotate a selected pawn
bool	pawn_rotation_r(world_t *world);

// rotate a selected pawn
bool	pawn_rotation_l(world_t *world);

// start the PvP offline
bool	start_game(world_t *world);
