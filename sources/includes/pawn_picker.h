#pragma once


#include "object.h"
#include "world.h"
#include "pawn.h"
#include "stdbool.h"

#include <SDL2/SDL.h>

#define	PICKER_BUT_SIDE(pwp)	((pwp->rect->h - 30) / 5)
#define POS_X_IN_PICKER(pwp, x) (pwp->rect->x +				       \
				(pwp->rect->w - 2 * PICKER_BUT_SIDE(pwp))      \
				/ 3					       \
				+ x * ((pwp->rect->w - 2 * PICKER_BUT_SIDE(pwp))\
				/ 3 + PICKER_BUT_SIDE(pwp)))
#define POS_Y_IN_PICKER(pwp, y) (	pwp->rect->y + 5 + y *		       \
					(5 + PICKER_BUT_SIDE(pwp)))

typedef struct		pawn_picker_s pawn_picker_t;
struct			pawn_picker_s
{
	obj_t		*but[10];
	SDL_Rect	*rect;

	int		pawn_count[2][5];
	pawn_color_t	col;
	int		status;
};

#include "square.h"

obj_t		*new_pawn_picker(world_t *world);
void		delete_pawn_picker(obj_t *pwp);
bool		set_pawn_but_color(world_t *world, pawn_picker_t *pwp);
char		*txr_path_pwn_picker_but(int x, int y);
button_type_t	type_pwn_picker_but(int x, int y);
int		valid_change(pawn_picker_t *picker, square_t *square);
void		apply_picker_change(	world_t		*world,
					pawn_picker_t	*picker,
					square_t	*square);
