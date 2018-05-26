#pragma once

#include <SDL2/SDL.h>

typedef struct			text_s text_t;
struct				text_s
{
	SDL_Texture		*txr;
	SDL_Rect		*rect;
	char			*info;
};

obj_t	*new_text(	world_t		*world,
			const char	*info,
			int		x,
			int		y,
			pos_type_t	ref);
void	set_text_rect(	text_t	*but,
			int		x,
			int		y,
			pos_type_t	ref);
void	set_text_txr(	world_t 	*world,
			text_t	*but,
			const char	*info);
void	del_text(obj_t *text);
