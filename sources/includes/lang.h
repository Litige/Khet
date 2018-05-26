#pragma once

typedef enum 	lang_e lang_t;
enum 		lang_e
{
	FR,
	EN,
	END_LANG
};

typedef enum 	text_e text_id_t;
enum 		text_e
{
	B_NEW_CAMP,
	B_CONT_CAMP,
	B_QUICK_GAME,
	B_RULES,
	B_SETTINGS,
	B_CREDITS,
	B_EXIT,
	B_BACK,
	B_VS_IA,
	B_VS_PL_ON,
	B_VS_PL_OFF,
	B_MAP_EDITOR,
	B_SAVE_EDIT,
	B_OPT_FULLSCREEN,
	B_OPT_SCREEN_SIZE,
	B_OPT_LANG,
	B_NAMER_OK,
	B_NAMER_BACK,
	T_MAIN_MENU,
	T_QUICK_GAME_MENU,
	T_SETTING_MENU,
	T_CREDIT_MENU,
	T_CREDIT_L1,
	T_CREDIT_L2,
	T_CREDIT_L3,
	T_CREDIT_L4,
	T_CREDIT_L5,
	T_DIF_CHOICE
};

/*
** TEXT[lang_t][text_t]
*/

static const char *TEXT[2][28] = {	{
						"Nouvelle Campagne\0",
						"Continuer Campagne\0",
						"Parti Rapide\0",
						"Règles\0",
						"Options\0",
						"Crédits\0",
						"Quitté\0",
						"Retour\0",
						"VS IA\0",
						"VS Joueur en ligne\0",
						"VS Joueur hors ligne\0",
						"Éditeur de Carte\0",
						"Enregistrer\0",
						"PLein Écran\0",
						"Taille de a fenêtre\0",
						"Langue\0",
						"Ok\0",
						"Retour\0",
						"Khet\0",
						"Choisissez un mode\0",
						"Options\0",
						"Crédits\0",
						"Khet, un projet de :\0",
						"Joshua Menanteau\0",
						"Arthur Froger\0",
						"Marcus Reynaud\0",
						"Kévin Desbonnets\0",
						"Choisissez une difficulté\0"
					},
					{
						"New Campaign\0",
						"Continue Campaign\0",
						"Quick Game\0",
						"Rules\0",
						"Options\0",
						"Credits\0",
						"Quit\0",
						"Back\0",
						"VS AI\0",
						"VS Player online\0",
						"VS Player offline\0",
						"Map Editor\0",
						"Save\0",
						"Fullscreen\0",
						"Window size\0",
						"Language\0",
						"Ok\0",
						"Back\0",
						"Khet\0",
						"Choose a game mode\0",
						"Settings\0",
						"Credits\0",
						"Khet, a project by :\0",
						"Joshua Menanteau\0",
						"Arthur Froger\0",
						"Marcus Reynaud\0",
						"Kévin Desbonnets\0",
						"Choose a difficulty\0"
					}
				};
