#include "laser.h"
#include "object.h"
#include "world.h"
#include "board.h"
#include "square.h"
#include "pawn.h"

#include "stdbool.h"

obj_t		*new_laser(world_t *world, board_t *board)
{

}

bool		gen_laser_txr()
{

}

bool		gen_laser_square(	laser_t		*laser,
					square_t	*square,
					int		*laser_status[2])
{
	int	l_orient;
	int	p_orient;

	if (	!square->pawn ||
		square->pawn->type == NO_PAWN)
	{
		*(laser_status)[0] = *(laser_status)[1];
		switch (*(laser_status)[1])
		{
			case NORTH:
				return(square->pos[0] == 0);
			case EAST:
				return(square->pos[1] == 9);
			case SOUTH:
				return(square->pos[0] == 7);
			case WEST:
				return(square->pos[1] == 0);
		}
	}

	l_orient = (*laser_status)[1];
	p_orient = square->pawn->orient;
	switch (square->pawn->type)
	{
		case PHARAOH:
			(*laser_status)[0] = -1;
			(*laser_status)[1] = -1;
			return(true);
		case SCARAB:
			if (p_orient % 2 == 0)
			{
				l_orient += (p_orient % 2 == 0) ? 1 : -1;
			}
			else
			{
				if (l_orient == 0 || l_orient == 3)
				{
					l_orient = (l_orient == 0) ? 3 : 0;
				}
				else
				{
					l_orient = (l_orient == 1) ? 2 : 1;
				}
			}
			break;
		case ANUBIS:
			p_orient = l_orient >= 2 ? -2 : 2;
			if (l_orient == p_orient)
			{
				l_orient = (*laser_status)[0];
			}
			else
			{

			}
			break;
		case PYRAMID:
			break;
		case SPHINX:
			l_orient = (*laser_status)[1];
			break;
		case NO_PAWN:
		default :
			(*laser_status)[0] = (*laser_status)[1];
			return(false);
	}

	(*laser_status)[0] = (*laser_status)[1];
	(*laser_status)[1] = l_orient;

	return(false);
}

void		delete_laser(obj_t *laser)
{

}
