
OBJS = $(wildcard sources/*.c)

CC = gcc

COMPILER_FLAGS = -w -Wall -Werror -Wextra

LINKER_FLAGS = -lSDL2_ttf $(shell sdl2-config --libs --cflags)

INCLUDE = -Isources/includes

OBJ_NAME = test

LIBS += $(LINKER_FLAGS)

build : $(OBJS)
	$(CC) -o $(OBJ_NAME) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE)

clean :
	rm -f $(wildcard *.o) $(wildcard vgcore.*) $(OBJ_NAME)

re :	clean build

all :
	re
