#pragma once

// error managment

typedef enum 	error_type_e error_type_t;
enum 		error_type_e
{
	DEFAULT,
	MALLOC,
	SDL_ER,
	TTF_ER
};

static error_type_t __errma = DEFAULT;

const char	*errma();
void		set_errma(error_type_t err);
