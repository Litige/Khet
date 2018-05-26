
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

	return(true);
}

bool			draw_s_line_to_surf(	int		point_1_x,
						int		point_1_y,
						int		point_2_x,
						int		point_2_y,
						SDL_Surface	*surface,
						Uint32		color)
{
	Uint32 		*target_pixel;
	int		min_x;
	int		max_x;
	int		min_y;
	int		max_y;


	if (point_1_x != point_2_x && point_1_y != point_2_y)
	{
		fprintf(stderr, "Warning : line isn't straight\n");
		return(true);
	}

	if (SDL_MUSTLOCK(surface))
	{
		if (SDL_LockSurface(surface) != 0)
		{
			set_errma(SDL_ER);
			return(false);
		}
	}

	if (point_1_y > point_2_y)
	{
		min_y = point_2_y > 0 ? point_2_y : 0;
		max_y = point_1_y < surface->h ? point_1_y : surface->h;
	}
	else if (point_1_y < point_2_y)
	{
		min_y = point_1_y > 0 ? point_1_y : 0;
		max_y = point_2_y < surface->h ? point_2_y : surface->h;
	}
	else
	{
		min_y = point_2_y;
		max_y = point_2_y + 1;
	}

	if (point_1_x > point_2_x)
	{
		min_x = point_2_x > 0 ? point_2_x : 0;
		max_x = point_1_x < surface->w ? point_1_x : surface->w;
	}
	else if (point_1_x < point_2_x)
	{
		min_x = point_1_x > 0 ? point_1_x : 0;
		max_x = point_2_x < surface->w ? point_2_x : surface->w;
	}
	else
	{
		min_x = point_1_x;
		max_x = point_1_x + 1;
	}

	for (int j = min_x; j < max_x; j++)
	{
		for (int i = min_y; i < max_y; i++)
		{
			// printf("pixel<%i,%i>\n", i, j);
			target_pixel = 	(Uint8 *) surface->pixels +
					i * surface->pitch +
					j * sizeof(*target_pixel);
			*target_pixel = color;
		}
	}


	if (SDL_MUSTLOCK(surface))
	{
		SDL_UnlockSurface(surface);
	}

	return(true);
}

bool			replace_color_in_surf(	SDL_Surface *surface,
						Uint32 target,
						Uint32 color)
{
	Uint32 		*target_pixel;

	if (SDL_LockSurface(surface) != 0)
	{
		set_errma(SDL_ER);
		return(false);
	}


	for (int y = 0; y < surface->w; y++)
	{
		for (int x = 0; x < surface->h; x++)
		{
			target_pixel = 	(Uint8 *) surface->pixels +
					y * surface->pitch +
					x * sizeof(*target_pixel);
			if (*target_pixel == target)
			{
				*target_pixel = color;
			}
		}
	}

	SDL_UnlockSurface(surface);

	return(true);
}

bool			render_rect(	SDL_Renderer	*rend,
					SDL_Rect	*rect,
					SDL_Color	color)
{
	if (	SDL_SetRenderDrawColor(	rend, color.r, color.g,
					color.b, color.a) < 0 		||
		SDL_RenderDrawRect(rend, rect) < 0			||
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	return(true);
}

bool			render_filled_rect(	SDL_Renderer	*rend,
						SDL_Rect	*rect,
						SDL_Color	color)
{
	if (	SDL_SetRenderDrawColor(	rend, color.r, color.g,
					color.b, color.a) < 0 		||
		SDL_RenderFillRect(rend, rect) < 0			||
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0) < 0)
	{
		set_errma(SDL_ER);
		return(false);
	}

	return(true);
}
