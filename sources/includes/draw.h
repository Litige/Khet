#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

bool			draw_rect_to_surf(	SDL_Rect	*src_rect,
						SDL_Surface	*surface,
						SDL_Rect	*dst_rect,
						Uint32		color);
bool			draw_s_line_to_surf(	int		point_1_x,
						int		point_1_y,
						int		point_2_x,
						int		point_2_y,
						SDL_Surface	*surface,
						Uint32		color);
bool			replace_color_in_surf(	SDL_Surface	*surface,
						Uint32		target,
						Uint32		color);
bool			render_rect(	SDL_Renderer	*rend,
					SDL_Rect	*rect,
					SDL_Color	color);
bool			render_filled_rect(	SDL_Renderer	*rend,
						SDL_Rect	*rect,
						SDL_Color	color);
