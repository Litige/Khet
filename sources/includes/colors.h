#pragma once

#include <SDL2/SDL.h>

/*
** Colors :					     {  R,   G,   B,    A}
*/

static const SDL_Color WHITE		= (SDL_Color){255, 255, 255, 255};
static const SDL_Color BLACK		= (SDL_Color){  0,   0,   0, 255};

static const SDL_Color BLUE		= (SDL_Color){  0,   0, 255, 255};
static const SDL_Color GREEN		= (SDL_Color){  0, 255,   0, 255};
static const SDL_Color RED		= (SDL_Color){255,   0,   0, 255};

static const SDL_Color GOLD		= (SDL_Color){255, 223,   0, 255};
static const SDL_Color DEEP_BLUE	= (SDL_Color){  0, 112, 179, 255};
static const SDL_Color TRANPARENCY	= (SDL_Color){  0,   0,   0,   0};


/*
** Set color masks according to the machine
*/

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	static const Uint32 r_mask	= 0xff000000;
	static const Uint32 g_mask	= 0x00ff0000;
	static const Uint32 b_mask	= 0x0000ff00;
	static const Uint32 a_mask	= 0x000000ff;
#else
	static const Uint32 r_mask	= 0x000000ff;
	static const Uint32 g_mask	= 0x0000ff00;
	static const Uint32 b_mask	= 0x00ff0000;
	static const Uint32 a_mask	= 0xff000000;
#endif
