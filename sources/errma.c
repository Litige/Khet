#include "errma.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
