
#include <stdbool.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "errma.h"

bool			draw_rect_to_surf(	SDL_Rect *src_rect,
						SDL_Surface *surface,
						SDL_Rect *dst_rect,
						Uint32 color)
{
	Uint32 		*target_pixel;
	int		min_x;
	int		max_x;
	int		min_y;
	int		max_y;

	if (SDL_LockSurface(surface) != 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	min_x = (src_rect->x - dst_rect->x > 0) ?
			0 : dst_rect->x - src_rect->x;
	max_x = ((src_rect->x + src_rect->w - dst_rect->x + dst_rect->w > 0) ?
			src_rect->w - min_x : dst_rect->w);
	min_y = (src_rect->y - dst_rect->y > 0) ?
			0 : dst_rect->y - src_rect->y;
	max_y = (src_rect->y + src_rect->h - dst_rect->y + dst_rect->h > 0) ?
			src_rect->h - min_y : dst_rect->h;

	for (int y =  min_y; y < max_y; y++)
	{
		for (int x = min_x; x < max_x; x++)
		{
			if	(y == min_y || y == max_y - 1 ||
				(x == min_x || x == max_x - 1))
			{
				target_pixel = 	(Uint8 *) surface->pixels +
						y * surface->pitch +
						x * sizeof(*target_pixel);
				*target_pixel = color;
			}
		}
	}

	SDL_UnlockSurface(surface);
}
