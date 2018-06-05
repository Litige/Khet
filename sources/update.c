#include "world.h"
#include "button.h"
#include "errma.h"
#include "text_obj.h"
#include "update.h"
#include "object.h"
#include "square.h"
#include "pawn.h"
#include "board.h"
#include "colors.h"
#include "pawn_picker.h"
#include "namer.h"
#include "dy_text.h"
#include "draw.h"
#include "laser.h"
#include "map_chooser.h"

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

int		update_elem(world_t *world, obj_t *elem)
{
	int	elem_status;

	switch (elem->obj_type)
	{
		case BUTTON_T :
			elem_status = update_button(world, elem->obj);
			break;
		case TEXT_T :
			elem_status = update_text(world, elem->obj);
			break;
		case BOARD_T :
			if (((board_t *)(elem->obj))->type != EDIT_MAP)
			{
				elem_status = update_board(world, elem->obj);
			}
			else
			{
				elem_status = update_map_edit(world, elem->obj);
			}
			break;
		case PAWN_PICKER_T:
			elem_status = update_pawn_picker(world, elem->obj);
			break;
		case NAMER_T :
			elem_status = update_namer(world, elem->obj);
			break;
		case DY_TEXT_T :
			elem_status = update_dy_text(world, elem->obj);
			break;
		case MAP_CHOOSER_T :
			elem_status = update_map_chooser(world, elem->obj);
			break;
		default:
			elem_status = 0;
			fprintf(stderr, "Warning : unrecognized object type\n");
			fprintf(stderr, "elem id : <%i>\n", elem->obj_type);
			break;
	}
	return(elem_status);
}

int		update_button(world_t *world, button_t *but)
{
	if (SDL_PointInRect((SDL_Point*)world->mouse_pos, but->rect))
	{
		but->over_in_func(world,but);
		but->overed = true;
	}
	else
	{
		if (but->overed)
		{
			but->over_out_func(world,but);
			but->overed = false;
		}
	}

	if (world->click && but->overed)
	{
		if (!but->button_func(world))
		{
			return(1);
		}
	}

	if (!but->txr)
	{
		fprintf(stderr,
			"Warning : missing texture on button at %dx%d\n",
			but->rect->x, but->rect->y);
	}
	else
	{
		if(SDL_RenderCopy(	world->renderer, but->txr,
					NULL, but->rect) < 0)
		{
			set_errma(SDL_ER);
			return(-1);
		}
	}

	return(0);
}

int		update_text(world_t *world, text_t *text)
{
	if(SDL_RenderCopy(	world->renderer, text->txr,
				NULL, text->rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}

	return(0);
}

int		update_map_edit(world_t *world, board_t *board)
{
	if(SDL_RenderCopy(	world->renderer, board->txr,
				NULL, board->rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			update_square_status(	world,
						board,
						board->squares[i][j]);
			if (draw_edit_square(	world,
						board,
						board->squares[i][j]) < 0)
			{
				return(-1);
			}
		}
	}

	return(0);
}

int		update_board(world_t *world, board_t *board)
{
	bool	target_rendered;

	target_rendered = false;
	if(SDL_RenderCopy(	world->renderer, board->txr,
				NULL, board->rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}

	if (	board->target_pos[0] != -1 && board->target_pos[1] != -1 &&
		IS_SET_FLAG(	board->squares
				[board->target_pos[1]]
				[board->target_pos[0]]->status, CLICKED))
	{
		set_but_pos(board);
		update_elem(world, board->rot_button[0]);
		update_elem(world, board->rot_button[1]);
	}

	if (!board->in_anime)
	{
		if (board->target_pos[1] != -1 && board->target_pos[0] != -1)
		{
			update_square_status(	world,
						board,
						board->squares
						[board->target_pos[1]]
						[board->target_pos[0]]);

		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (	i == board->target_pos[1] &&
					j == board->target_pos[0])
				{
					continue;
				}

				update_square_status(	world,
							board,
							board->squares[i][j]);
			}
		}
	}
	else if (board->in_anime && board->frame_nb == 0)
	{
		set_all_flag_to_no(board);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{

		if (	target_rendered			&&
			i == board->target_pos[1]	&&
			j == board->target_pos[0])
		{
			continue;
		}

			switch(draw_board_square(	world,
							board,
							board->squares[i][j]))
			{
				case 1:
					target_rendered = true;
					break;
				case -1:
					return(-1);
				case 0:
				default:
					break;
			}
		}
	}


	if (board->in_anime && board->frame_nb > 60 && world->click)
	{
		if (board->laser->target->pawn && board->laser->kill)
		{
			if (board->laser->target->pawn->type == PHARAOH)
			{
				world->dif	= board->dif;
				world->type	= board->type;
				world->level	= board->level;
				world->winner = board->laser->target
						->pawn->color == RED_PAWN ?
						BLUE_PLY : RED_PLY;
				load_end_game_scene(world);
				return(1);
			}
			delete_pawn(board->laser->target->pawn);
			board->laser->target->pawn = NULL;
		}
		delete_laser(board->laser);
		board->current_ply =	(board->current_ply == RED_PLY) ?
					BLUE_PLY : RED_PLY;
		board->turn	+= 1;
		board->in_anime = false;
		board->frame_nb = -1;
	}
	if (board->in_anime && board->frame_nb >= 0)
	{
		// to avoid potential overflow
		if (board->frame_nb <= 60)
		{
			board->frame_nb += 1;
		}

		if(SDL_RenderCopy(	world->renderer, board->laser->txr,
					NULL, board->laser->rect) < 0)
		{
			set_errma(SDL_ER);
			return(-1);
		}
	}
	if (board->in_anime && board->frame_nb < 0)
	{
		board->frame_nb += 1;
		board->target_pos[0] = -1;
		board->target_pos[1] = -1;

		board->laser = new_laser(world, board);
		if (!board->laser)
		{
			return(-1);
		}
	}

	return(0);
}

int		update_square_status(	world_t		*world,
					board_t		*board,
					square_t	*square)
{
	bool	was_clicked;

	was_clicked = IS_SET_FLAG(square->status, CLICKED);

	if (board->type == EDIT_MAP)
	{
		if (SDL_PointInRect((SDL_Point*)world->mouse_pos, square->rect))
		{
			ADD_FLAG(square->status, OVERED);
			if (world->click)
			{
				SWITCH_FLAG(square->status, CLICKED);
			}
		}
		else
		{
			DEL_FLAG(square->status, OVERED);
		}

		return(0);
	}

	if (SDL_PointInRect((SDL_Point*)world->mouse_pos, square->rect))
	{
		ADD_FLAG(square->status, OVERED);
		if (world->click && (valid_click(board, square) || was_clicked))
		{
			SWITCH_FLAG(square->status, CLICKED);
		}
	}
	else
	{
		DEL_FLAG(square->status, OVERED);
		if (was_clicked && world->click)
		{
			DEL_FLAG(square->status, CLICKED);
		}
	}

	if (valid_quit(world, board))
	{
		return(0);
	}

	if (	!valid_quit(world, board) &&
		was_clicked &&
		!IS_SET_FLAG(square->status, CLICKED))
	{
		set_all_move_target_flag(board, false);
		board->target_pos[0] = -1;
		board->target_pos[1] = -1;
	}
	else if (	!valid_quit(world, board) &&
			!was_clicked &&
			IS_SET_FLAG(square->status, CLICKED))
	{
		if (board->target_pos[0] > -1 && board->target_pos[1] > -1)
		{
			set_all_move_target_flag(board, false);
			board->target_pos[0] = -1;
			board->target_pos[1] = -1;
		}
		else
		{
			board->target_pos[0] = square->pos[0];
			board->target_pos[1] = square->pos[1];
			set_all_move_target_flag(board, true);
		}
	}

	return(0);
}

int		draw_edit_square(	world_t		*world,
					board_t		*board,
					square_t	*square)
{
	pawn_picker_t	*pwp;
	int		change;

	for (int i = 0; world->current_scene->elems[i] ; i++)
	{
		if (world->current_scene->elems[i]->obj_type = PAWN_PICKER_T)
		{
			pwp = world->current_scene->elems[i]->obj;
			break;
		}
	}

	if (!pwp)
	{
		return(-1);
	}

	change = valid_change(pwp, square);

	if (IS_SET_FLAG(square->status, OVERED))
	{
		if (change == 1)
		{
			if (!render_rect(world->renderer, square->rect, GREEN))
			{
				return(-1);
			}
		}
		else if (change == -1)
		{
			if (!render_rect(world->renderer, square->rect, RED))
			{
				return(-1);
			}
		}
		else
		{
			if (!render_rect(world->renderer, square->rect, WHITE))
			{
				return(-1);
			}
		}


		if (IS_SET_FLAG(square->status, CLICKED))
		{
			if (change == 1)
			{
				apply_picker_change(world, pwp, square);
			}
			DEL_FLAG(square->status, CLICKED);
		}
	}


	if (square->pawn)
	{
		return(display_pawn(world, square->pawn));
	}

	return(0);
}

int		draw_board_square(	world_t		*world,
					board_t		*board,
					square_t	*square)
{
	switch (square->status)
	{
		case OVERED	| CLICKED	| MOVE_TARGET:
			if (!TARGET_PAWN(board))
			{
				fprintf(stderr,
					"Warning : impossible case\n");
				fprintf(stderr,
					"No target pawn\n");
				fprintf(stderr,
					"OVERED & CLICKED & MOVE_TARGET\n");
				return(0);
			}

			if (!square->pawn)
			{
				swap_pawn(&TARGET_PAWN(board), &square->pawn);
				if (!render_filled_rect(world->renderer,
							board->squares
							[board->target_pos[1]]
							[board->target_pos[0]]
							->rect,
							BLACK) ||
					display_pawn(world, square->pawn) < 0)
				{
					swap_pawn(	&TARGET_PAWN(board),
							&square->pawn);
				}
				reset_pawn_rect(board, square->pawn,
						square->pos[1], square->pos[0]);
			}
			else
			{
				swap_pawn(&TARGET_PAWN(board), &square->pawn);
				reset_pawn_rect(board, square->pawn,
						square->pos[1], square->pos[0]);
				reset_pawn_rect(board, TARGET_PAWN(board),
						board->target_pos[1],
						board->target_pos[0]);
				if (	display_pawn(world, TARGET_PAWN(board))
					< 0 ||
					display_pawn(world, square->pawn) < 0)
				{
					swap_pawn(	&TARGET_PAWN(board),
							&square->pawn);
				}
			}

			board->in_anime = true;
			return(1);
			break;
		case OVERED	| 0		| MOVE_TARGET:
			if (!TARGET_PAWN(board))
			{
				fprintf(stderr, "Warning : impossible case\n");
				fprintf(stderr, "No target pawn\n");
				fprintf(stderr, "OVERED & MOVE_TARGET\n");
				return(0);
			}
			if (!square->pawn)
			{
				if (!render_filled_rect(world->renderer,
							board->squares
							[board->target_pos[1]]
							[board->target_pos[0]]
							->rect,
							BLACK))
				{
					swap_pawn_rect(	&TARGET_PAWN(board),
							&square->pawn);
					return(-1);
				}
				TARGET_PAWN(board)->rect->x =
						square->rect->x + 1;
				TARGET_PAWN(board)->rect->y =
						square->rect->y + 1;
				if (display_pawn(world, TARGET_PAWN(board)) < 0)
				{
					swap_pawn_rect(	&TARGET_PAWN(board),
							&square->pawn);
					return(-1);
				}
				reset_pawn_rect(board, TARGET_PAWN(board),
						board->target_pos[1],
						board->target_pos[0]);
			}
			else
			{
				swap_pawn_rect(	&TARGET_PAWN(board),
						&square->pawn);
				if (	display_pawn(world, TARGET_PAWN(board))
					< 0 ||
					display_pawn(world, square->pawn) < 0)
				{
					swap_pawn_rect(	&TARGET_PAWN(board),
							&square->pawn);
					return(-1);
				}
				swap_pawn_rect(	&TARGET_PAWN(board),
						&square->pawn);
			}
			return(1);
		case 0		| 0		| MOVE_TARGET:
			if (!render_rect(world->renderer, square->rect, GREEN))
			{
				return(-1);
			}
			if (square->pawn)
			{
				return(display_pawn(world, square->pawn));
			}
			break;
		case OVERED	| CLICKED	| 0:
		case OVERED	| 0		| 0:
			if (!render_rect(world->renderer, square->rect, GOLD))
			{
				return(-1);
			}
			if (square->pawn)
			{
				return(display_pawn(world, square->pawn));
			}
			break;
		case 0		| CLICKED	| 0:
		case 0		| 0		| 0:
			if (square->pawn)
			{
				return(display_pawn(world, square->pawn));
			}
			break;
		case 0		| CLICKED	| MOVE_TARGET:
			fprintf(stderr, "Warning : impossible case\n");
			fprintf(stderr, "CLICKED && MOVE_TARGET\n");
		default:
			if (square->pawn)
			{
				return(display_pawn(world, square->pawn));
			}
			break;
	}

	return(0);
}

int		update_pawn_picker(world_t *world, pawn_picker_t *pwp)
{
	for (int i = 0; i < 10; i++)
	{
		switch(update_button(world, pwp->but[i]->obj))
		{
			case -1:
				return(-1);
			case  1:
				return(1);
			case  0:
			default:
				break;
		}
	}

	if (!set_pawn_but_color(world, pwp))
	{
		return(-1);
	}

	if (!render_rect(world->renderer, pwp->rect, GOLD))
	{
		return(-1);
	}

	if (pwp->status != -1)
	{
		if (!render_rect(world->renderer,
				((button_t*)(pwp->but[pwp->status]->obj))
				->rect, GREEN))
		{
			return(-1);
		}
	}

	return(0);
}

int		update_namer(world_t *world, namer_t *namer)
{
	//	print all elem

	if (is_txt_input(world) && namer->ch_nb < 20)
	{
		strcat(namer->name, world->txt_input);
		namer->ch_nb += 1;
		namer->size_char[namer->ch_nb] = strlen(world->txt_input);

		if (!set_namer_txr_and_rect(world, namer))
		{
			return(-1);
		}
	}

	if (world->bk_space && namer->ch_nb > 0)
	{
		for (	int i = strlen(namer->name);
			namer->size_char[namer->ch_nb] > 0;
			namer->size_char[namer->ch_nb]--)
		{
			namer->name[i - namer->size_char[namer->ch_nb]]	 = 0;
		}
		namer->ch_nb -= 1;

		if (!set_namer_txr_and_rect(world, namer))
		{
			return(-1);
		}
	}

	if (!render_filled_rect(world->renderer, namer->rect, BLACK))
	{
		return(-1);
	}


	if (!render_rect(world->renderer, namer->rect, GOLD))
	{
		return(-1);
	}

	switch (update_elem(world, namer->ok_but))
	{
		case 0 :
			break;
		case 1 :
			return(1);
		case -1 :
			return(-1);
	}

	switch (update_elem(world, namer->back_but))
	{
		case 0 :
			break;
		case 1 :
			return(1);
		case -1 :
			return(-1);
	}

	if(	namer->txt_txr &&
		SDL_RenderCopy(	world->renderer, namer->txt_txr,
				NULL, namer->txt_rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}



	return(1);

}

int	update_dy_text(world_t *world, dy_text_t *text)
{
	if (!set_dy_text_txr(world, text) || !set_dy_text_rect(text))
	{
		return(-1);
	}

	if(SDL_RenderCopy(	world->renderer, text->txr,
				NULL, text->rect) < 0)
	{
		set_errma(SDL_ER);
		return(-1);
	}

	return(0);
}

int		update_map_chooser(world_t *world, map_chooser_t *mpc)
{
	int	display_target;

	display_target = mpc->target;

	for (int i = 0; mpc->names_txr[i]; i++)
	{
		if (SDL_RenderCopy(	world->renderer, mpc->names_txr[i],
					NULL, mpc->names_rect[i]) < 0)
		{
			set_errma(SDL_ER);
			return(-1);
		}
		if (SDL_PointInRect(	(SDL_Point*)world->mouse_pos,
					mpc->names_rect[i]))
		{
			if (!render_rect(	world->renderer,
						mpc->names_rect[i],
						GOLD))
			{
				return(-1);
			}
			if (world->click)
			{
				mpc->target =
					(mpc->target == mpc->top_map + i) ?
					-1 : mpc->top_map + i;
			}
			display_target = mpc->top_map + i;
		}
	}

	switch (update_elem(world, mpc->up_but))
	{
		case 0 :
			break;
		case 1 :
			return(1);
		case -1 :
			return(-1);
	}

	switch (update_elem(world, mpc->dw_but))
	{
		case 0 :
			break;
		case 1 :
			return(1);
		case -1 :
			return(-1);
	}

	if (display_target != -1)
	{
		if (SDL_RenderCopy(	world->renderer,
					mpc->maps_txr[display_target],
					NULL, mpc->txr_rect) < 0)
		{
			set_errma(SDL_ER);
			return(-1);
		}
	}

	if (!render_rect(world->renderer, mpc->txr_rect, GOLD))
	{
		return(-1);
	}

	if (!render_rect(world->renderer, mpc->select_rect, GOLD))
	{
		return(-1);
	}

	return(0);
}
