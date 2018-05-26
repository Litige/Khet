#pragma once

// error managment

typedef enum 	error_type_e error_type_t;
enum 		error_type_e
{
	DEFAULT,
	MALLOC,
	SDL_ER,
	TTF_ER,
	IMG_ER,
	GFX_ER,
	BAD_ARG
};

static error_type_t __errma = DEFAULT;

#define IS_ERRMA_SET (__errma != DEFAULT)

const char	*errma();
void		set_errma(error_type_t err);
void		get_errma();
