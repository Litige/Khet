
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "colors.h"
#include "text.h"
#include "draw.h"
#include "errma.h"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH (WINDOW_HEIGHT * 16 / 9)

SDL_Rect		center_rect_in_rect(	int text_w, int text_h,
						int surf_w, int surf_h)
{
	SDL_Rect	rect;

	rect.w = text_w;
	rect.h = text_h;
	rect.x = (surf_w - text_w) / 2;
	rect.y = (surf_h - text_h) / 2;

	return(rect);
}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
	TTF_Font *ttf_font, SDL_Texture **texture, SDL_Rect *rect)
{
	SDL_Surface *surface;
	SDL_Surface *txt_surface;
	SDL_Rect tmp_rect;

	surface = SDL_CreateRGBSurface(	0, 250, 50, 32,
					r_mask, g_mask, b_mask, a_mask);

	rect->x = x;
	rect->y = y;
	rect->w = 250;
	rect->h = 50;

	txt_surface = TTF_RenderText_Blended(ttf_font, text, GOLD);
	tmp_rect = center_rect_in_rect(	txt_surface->w, txt_surface->h,
					surface->w, surface->h);
	SDL_FillRect(	surface, NULL,
			SDL_MapRGB(	surface->format,
					DEEP_BLUE.r,
					DEEP_BLUE.g,
					DEEP_BLUE.b));


	SDL_BlitSurface(txt_surface, NULL, surface, &tmp_rect);

	// tmp_rect = *rect;
	//
	// draw_rect_to_surf(rect, surface, &tmp_rect, *((Uint32*)&WHITE));
	//
	// tmp_rect.x += 1;
	// tmp_rect.y += 1;
	// tmp_rect.w -= 1;
	// tmp_rect.h -= 1;
	//
	// draw_rect_to_surf(rect, surface, &tmp_rect, *((Uint32*)&WHITE));
	//
	// tmp_rect = *rect;
	// tmp_rect.x += 4;
	// tmp_rect.y += 4;
	// tmp_rect.w -= 4;
	// tmp_rect.h -= 4;
	//
	// draw_rect_to_surf(rect, surface, &tmp_rect, *((Uint32*)&WHITE));
	//
	// tmp_rect = *rect;
	// tmp_rect.x += 5;
	// tmp_rect.y += 5;
	// tmp_rect.w -= 5;
	// tmp_rect.h -= 5;
	//
	// draw_rect_to_surf(rect, surface, &tmp_rect, *((Uint32*)&WHITE));

	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void over(SDL_Renderer *renderer, int x, int y, char *text,
	TTF_Font *ttf_font, SDL_Texture **texture, SDL_Rect *rect)
{
	SDL_Surface *surface;
	SDL_Surface *txt_surface;
	SDL_Rect tmp_rect;

	surface = SDL_CreateRGBSurface(	0, 250, 50, 32,
					r_mask, g_mask, b_mask, a_mask);

	rect->x = x;
	rect->y = y;
	rect->w = 250;
	rect->h = 50;

	txt_surface = TTF_RenderText_Blended(ttf_font, text, WHITE);
	tmp_rect = center_rect_in_rect(	txt_surface->w, txt_surface->h,
					surface->w, surface->h);
	SDL_BlitSurface(txt_surface, NULL, surface, &tmp_rect);

	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

int main()
{
	SDL_Event	event;
	SDL_Rect	rect1, rect2;
	SDL_Renderer	*renderer;
	SDL_Texture	*texture1, *texture2;
	SDL_Window	*window;
	TTF_Font	*ttf_font;
	int quit;

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
	{
		printf("in file %s at line %d :\n", __FILE__, __LINE__ - 2);
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return(EXIT_FAILURE);
	}

	if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0,
					&window, &renderer) < 0)
	{
		printf("in file %s at line %d :\n", __FILE__, __LINE__ - 3);
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return(EXIT_FAILURE);
	}

	if (TTF_Init() < 0)
	{
		printf("in file %s at line %d :\n", __FILE__, __LINE__ - 2);
		fprintf(stderr, "error: %s\n", TTF_GetError());
		SDL_Quit();
		return(EXIT_FAILURE);
	}

	if ((ttf_font = TTF_OpenFont(MAIN_FONT, 30)) == NULL)
	{
		printf("in file %s at line %d :\n", __FILE__, __LINE__ - 2);
		fprintf(stderr, "error: %s\n", TTF_GetError());
		TTF_Quit();
		SDL_Quit();
		return(EXIT_FAILURE);
	}
	SDL_SetTextureBlendMode(&texture1, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(&texture2, SDL_BLENDMODE_BLEND);
	get_text_and_rect(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
			"toto est rigolo", ttf_font, &texture1, &rect1);
	over(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
			"toto est rigolo", ttf_font, &texture2, &rect1);
	// get_text_and_rect(renderer, WINDOW_WIDTH / 2, rect1.y + rect1.h,
	// 		"world", ttf_font, &texture2, &rect2);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderClear(renderer);
	/* Use textures. */


	quit = 0;
	int x = 0;
	int y = 0;
	SDL_Rect	tmp_rect;

	tmp_rect = rect1;
	tmp_rect.x += 2 ;
	tmp_rect.y += 2 ;
	tmp_rect.w -= 2 ;
	tmp_rect.h -= 2 ;



	while (!quit)
	{
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				quit = 1;
			}
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture1, NULL, &rect1);
			// SDL_RenderCopy(renderer, texture2, NULL, &rect2);
			SDL_GetMouseState(&x, &y);

			if (SDL_PointInRect(&((SDL_Point){x, y}), &rect1))
			{
				SDL_RenderCopy(renderer, texture2, NULL, &rect1);
				SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);

				tmp_rect = rect1;
				tmp_rect.x += 2 ;
				tmp_rect.y += 2 ;
				tmp_rect.w -= 4 ;
				tmp_rect.h -= 4 ;
				if (SDL_RenderDrawRect(renderer, &tmp_rect) < 0)
					fprintf(stderr, "warnning: %s\n", SDL_GetError());
				tmp_rect = rect1;
				tmp_rect.x += 3 ;
				tmp_rect.y += 3 ;
				tmp_rect.w -= 6 ;
				tmp_rect.h -= 6 ;
				if (SDL_RenderDrawRect(renderer, &tmp_rect) < 0)
					fprintf(stderr, "warnning: %s\n", SDL_GetError());

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				// printf("w = %d;\nh = %d\n", rect1.w, rect1.h);
				// printf("toto est rigolo\n");
			}
			else
			{
				get_text_and_rect(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
						"toto est rigolo", ttf_font, &texture1, &rect1);
			}
			SDL_RenderPresent(renderer);
		}
	}

	/* Deinit TTF. */
	SDL_DestroyTexture(texture1);
	// SDL_DestroyTexture(texture2);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
