#pragma once

#define SMA_FONT 10	//world->size[1] / 10
#define MED_FONT 50	//world->size[1] / 5
#define BIG_FONT 100	//world->size[1] / 2

#ifdef _WIN32

static const char MAIN_FONT[] = "ressources\FantasqueSansMono-Regular.ttf";
static const char SECONDARY_FONT[] = "ressources\Aaargh.ttf";

#else

static const char MAIN_FONT[] = "ressources/FantasqueSansMono-Regular.ttf";
static const char SECONDARY_FONT[] = "ressources/Aaargh.ttf";

#endif
