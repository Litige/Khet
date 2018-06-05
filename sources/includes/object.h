#pragma once

typedef enum 	obj_type_e obj_type_t;
enum 		obj_type_e
{
	BUTTON_T,
	TEXT_T,
	BOARD_T,
	PAWN_PICKER_T,
	NAMER_T,
	DY_TEXT_T,
	MAP_CHOOSER_T
};

typedef struct			obj_s obj_t;
struct				obj_s
{
	obj_type_t		obj_type;
	void			*obj;
};

typedef enum 	pos_type_s pos_type_t;
enum 		pos_type_s
{
	CENTER,
	TOPLEFT
};
