#pragma once

typedef enum 	lang_e lang_t;
enum 		lang_e
{
	FR,
	EN,
	END_LANG
};

typedef enum 	text_e text_t;
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
	B_OPT_FULLSCREEN,
	B_OPT_SCREEN_SIZE,
	B_OPT_LANG
};

/*
** TEXT[lang_t][text_t]
*/

static const char *TEXT[2][14] = {	{
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
						"PLein Écran\0",
						"Taille de a fenêtre\0",
						"Langue\0"
					},
					{	"New Campaign\0",
						"Continue Campaign\0",
						"Quick Game\0",
						"Rules\0",
						"Options\0",
						"Credits\0",
						"Quit\0",
						"Retour\0",
						"VS AI\0",
						"VS Player online\0",
						"VS Player offline\0",
						"Fullscreen\0",
						"Window size\0",
						"Language\0"
					}
				};
