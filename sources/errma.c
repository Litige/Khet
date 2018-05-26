#include "errma.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

const char	*errma()
{
	switch(__errma)
	{
		case MALLOC:
			return("malloc couldn't allocate memmory");
			break;
		case SDL_ER:
			return(SDL_GetError());
			break;
		case TTF_ER:
			return(TTF_GetError());
			break;
		case IMG_ER:
			return(IMG_GetError());
			break;
		case GFX_ER:
			return("a SDL_gfx error occured");
			break;
		case BAD_ARG:
			return("bad argument where given, fatal error");
			break;
		case DEFAULT:
		default :
			return("");
	}

	return("errma not correctly set");
}

void		set_errma(error_type_t err)
{
	__errma = err;
}

void		get_errma()
{
	return(__errma);
}
