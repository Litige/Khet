#pragma once

typedef enum 	lang_e lang_t;
enum 		lang_e
{
	FR,
	EN
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
	B_EXIT
};

/*
** TEXT[lang_t][text_t]
*/

static const char *TEXT[2][7] = {	{	"Nouvelle Campagne\0",
						"Continuer Campagne\0",
						"Parti Rapide\0",
						"Règles\0",
						"Options\0",
						"Crédits\0",
						"Quitté\0"},
					{	"New Campaign\0",
						"Continue Campaign\0",
						"Quick Game\0",
						"Rules\0",
						"Options\0",
						"Credits\0",
						"Quit\0"}}		;
