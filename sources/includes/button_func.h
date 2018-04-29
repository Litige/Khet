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
