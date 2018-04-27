#pragma once

#include "world.h"

// NEW_CAMP button
void				load_difficulty_scene(world_t *world);
// CONT_CAMP button
void				continue_campaign(world_t *world);
// EASY_DIF button
void				set_diff_easy(world_t *world);
// MED_DIF button
void				set_diff_medium(world_t *world);
// HARD_DIF button
void				set_diff_hard(world_t *world);

// QUICK_GAME button
void				load_quick_game_menu_scene(world_t *world);
// VS_AI button
void				vs_ai(world_t *world);
// VS_PLY_ON button
void				vs_player_offline(world_t *world);
// VS_PLY_OFF button
void				vs_player_online(world_t *world);

// RULES button
void				load_rules_scene(world_t *world);
// TUTO button
void				start_tutorial(world_t *world);

// BACK button
void				load_main_menu_scene(world_t *world);
// EXIT button
void				quit_game(world_t *world);
// SETTINGS button
void				load_settings_scene(world_t *world);

// used for usseless button
void				dummy_function(world_t *world);
