#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <constant.h>

struct game {
	struct map** maps;       // the game's map - tableau des maps
	short max_levels;        // nb maps of the game
	short current_level;
	struct player* player;
	struct bomb* bombs;
};

struct game_infos
{
	short max_levels;
	short current_level;
	int current_x;
	int current_y;
	char * map_prefix;
};

struct map {
	int width;
	int height;
	unsigned char* grid;
};


struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	short nb_life;
	short nb_keys;
	int range;
};

struct bomb {
  int range;
  int x, y;
  short current_level;
  unsigned int time_init;
  int state;
  short mortality;
  struct bomb *next;
};

#endif /* INCLUDE_STRUCTURES_H_ */
