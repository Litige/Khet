#pragma once

#include "world.h"
#include "board.h"
#include "object.h"
#include "lang.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef enum 	data_type_s data_type_t;
enum 		data_type_s
{
	STRING,
	CHAR,
	INT,
	PLAYER
};

typedef struct			dy_text_s dy_text_t;
struct				dy_text_s
{
	SDL_Texture		*txr;
	SDL_Rect		*rect;

	char			*info;		// string that doesn't change
	char			*value;		// string being updated

	data_type_t		data_type;
	void			*data;
};


obj_t	*new_dy_text(	world_t		*world,
			const char	*info,
			void		*data,
			data_type_t	data_type,
			int		x,
			int		y,
			pos_type_t	ref);
bool	set_dy_text_rect(dy_text_t *text);
bool	set_dy_text_txr(world_t *world, dy_text_t *text);
void	del_dy_text(obj_t *text);
bool	set_text_value(	world_t		*world,
			char		**full_text,
			char		**new_value,
			dy_text_t	*text);
