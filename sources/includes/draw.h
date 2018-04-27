#pragma once

#include <stdbool.h>
#include <unistd.h>

#include <SDL2/SDL.h>

bool			draw_rect_to_surf(	SDL_Rect *src_rect,
						SDL_Surface *surface,
						SDL_Rect *dst_rect,
						Uint32 color);
