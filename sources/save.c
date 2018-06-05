#include "world.h"
#include "board.h"
#include "square.h"
#include "pawn.h"
#include "save.h"
#include "errma.h"

#include <stdio.h>

bool		map_loading(world_t *world, char map[8][10][4])
{
	bool	ret_val;

	switch (world->type)
	{
		case PLY_VS_PLY_ONLINE :
			// will do nothing for now
			// would send message to other player
			// the ID of the map
			ret_val = load_map(map, "Classic");
			break;
		case PLY_VS_PLY_LOCAL :
		case PLY_VS_IA :
			printf("%s\n", world->map);
			if (world->map)
			{
				ret_val = load_map(map, world->map);
			}
			else
			{
				ret_val = load_map(map, "Classic");
			}
			break;
		case NEW_CAMPAIGN :
			switch(world->level)
			{
				case LEVEL_2:
					ret_val = load_map(map, "Imhotep");
					break;
				case LEVEL_3:
					ret_val = load_map(map, "Dynasty");
					break;
				case LEVEL_1:
				default:
					ret_val = load_map(map, "Classic");
					break;
			}
			break;
		case LOAD_CAMPAIGN :
			ret_val = true;
			// load nothing
			// load_save is called later on in new_board
			break;
		case EDIT_MAP :
			empty_map(map);
			break;
		case NO_TYPE :
		default :
			return(false);
	}

	return(ret_val);
}

bool		save_map(board_t *brd, char *name)
{
	FILE	*file;
	int	error_val;
	char	*full_path;

	if (!(full_path = malloc(	sizeof(char) *
					(strlen(MAP_P) + 4 + strlen(name)))))
	{
		set_errma(MALLOC);
		return(false);
	}

	if (sprintf(full_path, "%s%s.map", MAP_P, name) < 0)
	{
		perror("error, couldn't save map");
		return(false);
	}

	error_val = 0;
	if (!(file = fopen(full_path, "w")))
	{
		perror("error, couldn't save map");
		return(false);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			error_val = fprintf(	file, "%i%i%i%i",
						brd->squares[i][j]->type,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->type	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->orient	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->color	: 0);
			if (error_val < 0)
			{
				perror("error, couldn't save map");
				return(false);
			}
		}
	}

	if (fclose(file) == EOF)
	{
		perror("error, couldn't save map");
		return(false);
	}

	return(true);
}

bool		load_map(char map[8][10][4], char *name)
{
	FILE	*file;
	int	error_val;
	char	*full_path;

	if (!name)
	{
		errma(BAD_ARG);
		return(false);
	}

	if (!(full_path = malloc(	sizeof(char) *
					(strlen(MAP_P) + strlen(name)+ 4 + 1))))
	{
		set_errma(MALLOC);
		return(false);
	}

	if (strcmp(name, "autosave") == 0)
	{
		if (sprintf(full_path, "autosave\0") < 0)
		{
			perror("error, couldn't save map");
			return(false);
		}
	}
	else if (sprintf(full_path, "%s%s.map", MAP_P, name) < 0)
	{
		perror("error, couldn't save map");
		return(false);
	}

	error_val = 0;
	if (!(file = fopen(full_path, "r")))
	{
		perror("error, couldn't load map");
		return(false);
	}

	// avoid reading game info if loading map from save
	if (strcmp(name, "autosave") == 0)
	{
		if (fseek(file, sizeof(char) * 8, SEEK_SET) != 0)
		{
			if (ferror(file))
			{
				perror("error, couldn't load map");
				return(false);
			}
		}
	}


	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			error_val = fread(map[i][j], sizeof(char), 4, file);
			if (error_val < 0)
			{
				perror("error, couldn't load map");
				return(false);
			}
			map[i][j][0] -= '0';
			map[i][j][1] -= '0';
			map[i][j][2] -= '0';
			map[i][j][3] -= '0';
		}
	}


	if (fclose(file) == EOF)
	{
		perror("error, couldn't load map");
		return(false);
	}

	return(true);
}

bool		save_game(board_t *brd)
{
	FILE	*file;
	int	error_val;

	error_val = 0;
	if (!(file = fopen("autosave", "w")))
	{
		perror("error, couldn't save game");
		return(false);
	}

	error_val = fprintf(	file, "%i%i%i%i%04i",
				brd->dif,
				brd->type,
				brd->level,
				brd->current_ply,
				brd->turn);
	if (error_val < 0)
	{
		perror("error, couldn't save game");
		return(false);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			error_val = fprintf(	file, "%i%i%i%i",
						brd->squares[i][j]->type,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->type	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->orient	: 0,
	(brd->squares[i][j]->pawn)? brd->squares[i][j]->pawn->color	: 0);
			if (error_val < 0)
			{
				perror("error, couldn't save game");
				return(false);
			}
		}
	}

	if (fclose(file) == EOF)
	{
		perror("error, couldn't save game");
		return(false);
	}

	return(true);
}

bool		load_save(board_t *board, char map[8][10][4])
{
	FILE	*file;
	char	param[8];

	if (!(file = fopen("autosave", "r")))
	{
		perror("error, couldn't load save");
		return(false);
	}

	if (fread(param, sizeof(char), 8, file) < 0)
	{
		perror("error, couldn't load save");
		return(false);
	}

	if (fclose(file) == EOF)
	{
		perror("error, couldn't load save");
		return(false);
	}


	board->dif		= param[0] - '0';
	board->type		= param[1] - '0';
	board->level		= param[2] - '0';
	board->current_ply	= param[3] - '0';
	board->turn		=	(param[4] - '0') * 1000 +
					(param[5] - '0') * 100 +
					(param[6] - '0') * 10 +
					(param[7] - '0');

	if (load_map(map, "autosave"))
	{
		if (remove("autosave") < 0)
		{
			perror("error, couldn't delete autosave ");
			return(false);
		}
		return(true);
	}
	else
	{
		return(false);
	}
}


void	set_square_layout(char map[8][10][4])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j][0] = BLANK_SQUARE;
		}
		map[i][0][0] = RED_SQUARE;
		map[i][9][0] = BLUE_SQUARE;
		if (i == 0 || i == 7)
		{
			map[i][8][0] = RED_SQUARE;
			map[i][1][0] = BLUE_SQUARE;
		}
	}
}

void		empty_map(char map[8][10][4])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j][0] = 0;
			map[i][j][1] = 0;
			map[i][j][2] = 0;
			map[i][j][3] = 0;
		}
	}
}
