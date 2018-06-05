
#include "map_chooser.h"
#include "world.h"
#include "object.h"
#include "button.h"
#include "pawn.h"
#include "save.h"
#include "errma.h"
#include "images.h"
#include "dirent.h"
#include "colors.h"

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>

obj_t			*new_map_chooser(world_t *world)
{
	map_chooser_t	*new_mpc;
	obj_t		*new_obj;

	if (	!(new_mpc		= malloc(sizeof(map_chooser_t)))||
		!(new_obj		= malloc(sizeof(obj_t)))	||
		!(new_mpc->txr_rect	= malloc(sizeof(SDL_Rect)))	||
		!(new_mpc->select_rect	= malloc(sizeof(SDL_Rect))))
	{
		set_errma(MALLOC);
		return(NULL);
	}

	new_obj->obj		= new_mpc;
	new_obj->obj_type	= MAP_CHOOSER_T;

	for (int i = 0; i < 5; i++)
	{
		if (!(new_mpc->names_rect[i] = malloc(sizeof(SDL_Rect))))
		{
			set_errma(MALLOC);
			return(NULL);
		}
	}

	new_mpc->maps_txr	= NULL;
	new_mpc->maps_name	= NULL;
	new_mpc->up_but		= NULL;
	new_mpc->dw_but		= NULL;

	new_mpc->target		= -1;
	new_mpc->top_map	=  0;
	new_mpc->map_nb		=  0;

	new_mpc->up_but = new_button(	world, UP_MCH, TXR_IMAGE, UP_MCH_BUT,
					0, 0, TOPLEFT);
	new_mpc->dw_but = new_button(	world, DW_MCH, TXR_IMAGE, DW_MCH_BUT,
					0, 0, TOPLEFT);

	if (!new_mpc->up_but || !new_mpc->dw_but)
	{
		printf("failed at button creation\n");
		del_map_chooser(new_obj);
		return(NULL);
	}

	for (int i = 0; i < 5; i++)
	{
		new_mpc->names_txr[i] = NULL;
	}

	set_all_rect(world, new_mpc);

	if (	(new_mpc->map_nb = get_map_number()) == -1	||
		!set_all_names(new_mpc)				||
		!set_names_txr_and_rect(world, new_mpc)		||
		!set_all_txr(world, new_mpc))
	{
		del_map_chooser(new_obj);
		return(NULL);
	}

	return(new_obj);
}

bool			set_names_txr_and_rect(	world_t		*world,
						map_chooser_t	*mpc)
{
	SDL_Surface	*surf;

	for (	Uint8 i = mpc->top_map;
		mpc->maps_name[i] && i < mpc->top_map + 5;
		i++)
	{
		surf = TTF_RenderUTF8_Blended(	world->ttf_font,
						mpc->maps_name[i],
						GOLD);
		if (!surf)
		{
			set_errma(TTF_ER);
			return(false);
		}

		if (mpc->names_txr[i - mpc->top_map])
		{
			SDL_DestroyTexture(mpc->names_txr[i - mpc->top_map]);
		}

		mpc->names_txr[i - mpc->top_map] =
			SDL_CreateTextureFromSurface(world->renderer, surf);
		SDL_FreeSurface(surf);
		if (mpc->names_txr[i - mpc->top_map] == NULL)
		{
			set_errma(SDL_ER);
			return(false);
		}

		if (SDL_QueryTexture(	mpc->names_txr[i - mpc->top_map],
					NULL, NULL,
					&mpc->names_rect[i - mpc->top_map]->w,
					&mpc->names_rect[i - mpc->top_map]->h)
					< 0)
		{
			set_errma(SDL_ER);
			return(false);
		}

		mpc->names_rect[i - mpc->top_map]->x =
			mpc->select_rect->x + 5 +
			(mpc->names_rect[i - mpc->top_map]->w / 2);
	}

	return(true);
}

bool		set_all_rect(world_t *world, map_chooser_t *map_chooser)
{
	map_chooser->txr_rect->h = world->size[0] - 10;
	map_chooser->txr_rect->w = 	(world->size[0] - 10) +
					((world->size[0] - 10) / 4);
	// make sure	map_chooser->txr_rect->h % 8  == 0
	// and		map_chooser->txr_rect->w % 10 == 0
	map_chooser->txr_rect->h = (map_chooser->txr_rect->h / 8) * 8;
	map_chooser->txr_rect->w = (map_chooser->txr_rect->w / 10) * 10;

	map_chooser->txr_rect->x = 	world->size[1] -
					(map_chooser->txr_rect->w + 5);
	map_chooser->txr_rect->y = 5;

	map_chooser->select_rect->x = 5;
	map_chooser->select_rect->y = 5;
	map_chooser->select_rect->w =	world->size[1] -
					(15 + map_chooser->txr_rect->w);
	map_chooser->select_rect->h =  (world->size[0] / 10) * 7 + 40;

	for (int i = 0; i < 5; i++)
	{
		map_chooser->names_rect[i]->w =
				map_chooser->select_rect->w - 10;
		map_chooser->names_rect[i]->h =
				(map_chooser->select_rect->h - 40) / 7 -
				(map_chooser->select_rect->h - 40) % 7;

		map_chooser->names_rect[i]->x =
				map_chooser->select_rect->x + 5;
		map_chooser->names_rect[i]->y =
				map_chooser->select_rect->y + 5 +
				(map_chooser->names_rect[i]->h + 5) * (1 + i);
	}

	((button_t*)(map_chooser->up_but->obj))->rect->x =
					map_chooser->select_rect->x +
					map_chooser->select_rect->w / 2 -
					map_chooser->names_rect[0]->h / 2;
	((button_t*)(map_chooser->up_but->obj))->rect->y =
					map_chooser->select_rect->y + 5;

	((button_t*)(map_chooser->up_but->obj))->rect->w =
					map_chooser->names_rect[0]->h;
	((button_t*)(map_chooser->up_but->obj))->rect->h =
					map_chooser->names_rect[0]->h;

	((button_t*)(map_chooser->dw_but->obj))->rect->x =
					map_chooser->select_rect->x +
					map_chooser->select_rect->w / 2 -
					map_chooser->names_rect[0]->h / 2;
	((button_t*)(map_chooser->dw_but->obj))->rect->y =
					map_chooser->select_rect->y +
					map_chooser->select_rect->h -
					(map_chooser->names_rect[0]->h + 5);

	((button_t*)(map_chooser->dw_but->obj))->rect->w =
					map_chooser->names_rect[0]->h;
	((button_t*)(map_chooser->dw_but->obj))->rect->h =
					map_chooser->names_rect[0]->h;

	return(true);
}

bool		set_all_names(map_chooser_t *map_chooser)
{

	struct dirent	*new_dir_entry;
	DIR		*dir;
	Uint8		i;

	if ((dir = opendir("map")) == NULL)
	{
		perror("opendir");
		fprintf (stderr, "Cannot open ressources directory\n");
		return(false);
	}

	if (!(	map_chooser->maps_name =
		malloc(sizeof(char*) * (map_chooser->map_nb + 1))))
	{
		set_errma(MALLOC);
		return(false);
	}

	memset(	map_chooser->maps_name, 0,
		sizeof(char*) * (map_chooser->map_nb + 1));

	i = 0;

	while ((new_dir_entry = readdir(dir)))
	{

		if(	new_dir_entry->d_type == DT_REG &&
			is_a_map(new_dir_entry->d_name))
		{
			if (!(	map_chooser->maps_name[i] =
				malloc(sizeof(char*) *
				(strlen(new_dir_entry->d_name) - 4 + 1))))
			{
				set_errma(MALLOC);
				return(false);
			}

			memset(	map_chooser->maps_name[i],
				0,
				sizeof(char) *
				(strlen(new_dir_entry->d_name) - 4 + 1));
			strncat(map_chooser->maps_name[i],
				new_dir_entry->d_name,
				strlen(new_dir_entry->d_name) - 4);

			i++;
		}
	}

	closedir(dir);

	return(true);
}

bool		set_all_txr(world_t *world, map_chooser_t *map_chooser)
{
	char	map[8][10][4];

	empty_map(map);

	if (!(	map_chooser->maps_txr =
		malloc(sizeof(SDL_Texture*) * (map_chooser->map_nb + 1))))
	{
		set_errma(MALLOC);
		return(NULL);
	}

	memset(	map_chooser->maps_txr, 0,
		sizeof(SDL_Texture*) * (map_chooser->map_nb + 1));

	for (Uint8 i = 0; i < map_chooser->map_nb; i++)
	{
		if (!load_map(map, map_chooser->maps_name[i]))
		{
			printf("failed at map\n");
			return(false);
		}

		if (!(map_chooser->maps_txr[i] = get_txr_from_map(
								world,
								map_chooser,
								map)))
		{
			return(false);
		}
	}

	return(true);
}

Uint8		get_map_number()
{
	struct dirent	*new_dir_entry;
	DIR		*dir;
	Uint8		map_nb;

	if ((dir = opendir("map")) == NULL)
	{
		perror("opendir");
		fprintf (stderr, "Cannot open ressources directory\n");
		return(-1);
	}

	map_nb = 0;

	while ((new_dir_entry = readdir(dir)))
	{
		if(	new_dir_entry->d_type == DT_REG &&
			is_a_map(new_dir_entry->d_name))
		{
			map_nb += 1;
		}
	}

	closedir(dir);

	return(map_nb);
}

SDL_Texture	*get_txr_from_map(	world_t		*world,
					map_chooser_t	*map_chooser,
					const char	map[8][10][4])
{
	SDL_Surface	*surf;
	SDL_Texture	*txr;
	SDL_Rect	square_rect;
	SDL_Rect	pawn_rect;
	int		tmp;

	square_rect.x = 0;
	square_rect.y = 0;
	square_rect.h = map_chooser->txr_rect->h / 8;
	square_rect.w = map_chooser->txr_rect->h / 8;

	pawn_rect.x = square_rect.x + 1;
	pawn_rect.y = square_rect.y + 1;
	pawn_rect.h = map_chooser->txr_rect->h / 8 - 2;
	pawn_rect.w = map_chooser->txr_rect->h / 8 - 2;

	surf = SDL_CreateRGBSurface(	0,
					map_chooser->txr_rect->w,
					map_chooser->txr_rect->h, 32,
					r_mask, g_mask, b_mask, a_mask);

	if (!surf)
	{
		set_errma(SDL_ER);
		return(NULL);
	}

	if (SDL_SetColorKey(surf, SDL_TRUE, *((Uint32*)(&TRANPARENCY))) < 0)
	{
		set_errma(SDL_ER);
		return(NULL);
	}

	for (int i = 0; i < 8; i++)
	{
		square_rect.y	= square_rect.h * i;
		for (int j = 0; j < 10; j++)
		{
			square_rect.x = square_rect.w * j;

			if (map[i][j][0] != BLANK_SQUARE)
			{
				tmp = SDL_FillRect(
						surf,
						&square_rect,
				(map[i][j][0] == RED_SQUARE) ?
				*((Uint32*)&ST_RED) : *((Uint32*)&ST_BLUE));
			}
			else
			{
				tmp = SDL_FillRect(	surf, &square_rect,
							*((Uint32*)&BLACK));
			}
			if (map[i][j][1] != NO_PAWN)
			{
				pawn_rect.y	= square_rect.y + 1;
				pawn_rect.x	= square_rect.x + 1;
				tmp = blit_pawn_to_surf(world, surf,
							&pawn_rect,
							map[i][j][1],
							map[i][j][3],
							map[i][j][2]);
			}

			if (tmp < 0)
			{
				set_errma(SDL_ER);
				SDL_FreeSurface(surf);
				return(NULL);
			}
		}
	}

	txr = SDL_CreateTextureFromSurface(world->renderer, surf);
	SDL_FreeSurface(surf);

	if (txr == NULL)
	{
		set_errma(SDL_ER);
	}

	return(txr);
}

int			blit_pawn_to_surf(	world_t			*world,
						SDL_Surface		*surf,
						SDL_Rect		*rect,
						pawn_type_t		type,
						pawn_color_t		color,
						pawn_orientation_t	orient)
{
	SDL_Surface	*pawn;
	SDL_Surface	*tmp;


	switch (type)
	{
		case PHARAOH:
			pawn = IMG_Load(PHARAOH_PAWN);
			break;
		case SCARAB:
			pawn = IMG_Load(SCARAB_PAWN);
			break;
		case ANUBIS:
			pawn = IMG_Load(ANUBIS_PAWN);
			break;
		case PYRAMID:
			pawn = IMG_Load(PYRAMID_PAWN);
			break;
		case SPHINX:
			pawn = IMG_Load(SPHINX_PAWN);
			break;
		case NO_PAWN:
		default:
			return(-1);

	}

	if (!pawn)
	{
		set_errma(IMG_ER);
		return(-1);
	}


	if (!swap_color(world, pawn, color))
	{
		SDL_FreeSurface(pawn);
		return(-1);
	}

	tmp = pawn;

	if (orient != NORTH)
	{
		if ((pawn = rotateSurface90Degrees(pawn, orient)) == NULL)
		{
			set_errma(GFX_ER);
			SDL_FreeSurface(pawn);
			return(-1);
		}
		else
		{
			SDL_FreeSurface(tmp);
		}
	}


	if (SDL_BlitScaled(pawn, NULL, surf, rect) < 0)
	{
		set_errma(SDL_ER);
		SDL_FreeSurface(pawn);
		return(-1);
	}

	return(0);
}

void		del_map_chooser(obj_t *mpc)
{
	if (mpc)
	{
		if (mpc->obj)
		{
			free_txr_and_name(mpc->obj);
			free_rect(mpc->obj);
			if (((map_chooser_t*)(mpc->obj))->up_but)
			{
				del_button(((map_chooser_t*)(mpc->obj))
						->up_but);
			}
			if (((map_chooser_t*)(mpc->obj))->dw_but)
			{
				del_button(((map_chooser_t*)(mpc->obj))
						->dw_but);
			}
		}
		free(mpc);
	}
}

void 		free_txr_and_name(map_chooser_t* mpc)
{
	for(	Uint8 i = 0; i < mpc->map_nb; i++)
	{
		if (mpc->maps_txr && mpc->maps_txr[i])
		{
			SDL_DestroyTexture(mpc->maps_txr[i]);
			mpc->maps_txr[i] = NULL;
		}
		if (mpc->maps_name && mpc->maps_name[i])
		{
			free(mpc->maps_name[i]);
			mpc->maps_name[i] = NULL;
		}
	}
}

void 		free_rect(map_chooser_t* mpc)
{
	for (int i = 0; i < 5; i++)
	{
		if (mpc->names_rect[i])
		{
			free(mpc->names_rect[i]);
			mpc->names_rect[i] = NULL;
		}
	}

	if (mpc->select_rect)
	{
		free(mpc->select_rect);
		mpc->select_rect = NULL;
	}

	if (mpc->txr_rect)
	{
		free(mpc->txr_rect);
		mpc->txr_rect = NULL;
	}
}

bool		is_a_map(char *filename)
{
	int	len = strlen(filename);

	for (int i = len - 1; i > 0; i--)
	{
		if (	filename[i]	== '.'	&&
			len - i		>=  3	&&
			filename[i + 1] == 'm'	&&
			filename[i + 2] == 'a'	&&
			filename[i + 3] == 'p')
		{
			return(true);
		}
	}

	return(false);
}
