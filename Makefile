
OBJS = $(wildcard sources/*.c)

CC = gcc

COMPILER_FLAGS = -w -Wall -Werror -Wextra

LINKER_FLAGS = -lSDL2_ttf -lSDL2_image -lSDL2_gfx $(shell sdl2-config --libs --cflags)

INCLUDE = -Isources/includes

OBJ_NAME = Khet

LIBS += $(LINKER_FLAGS)

build : $(OBJS)
	$(CC) -o $(OBJ_NAME) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE) 2> compil.log

clean :
	rm -f $(wildcard vgcore.*) $(OBJ_NAME)

re :	clean build

all :
	re
