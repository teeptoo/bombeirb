#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <constant.h>

struct game {
	struct map** maps;       // the game's map - tableau des maps
	short nb_levels;        // nb maps of the game
	short current_level;
	struct player* player;
	struct bomb* bombs;
};

struct game_infos
{
	short nb_levels;
	char * map_prefix;
};

struct map {
	int width, height;
	int starting_x, starting_y;
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
