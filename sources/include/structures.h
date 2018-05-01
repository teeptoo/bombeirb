#ifndef STRUCTURES_H_
#define STRUCTURES_H_


#include <constant.h>

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70 	   		// 	0111 0000

};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum scenery_type {
	SCENERY_STONE,     // 0000
	SCENERY_TREE,      // 0001
	SCENERY_PRINCESS   // 0010
};

enum door_type {
	DOOR_CLOSED,		// 0000
	DOOR_OPENED			// 0001
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_MONSTER     = CELL_BOX | BONUS_MONSTER,
    CELL_BOX_LIFE  = CELL_BOX | BONUS_LIFE,

	CELL_DOOR_CLOSED  = CELL_DOOR | DOOR_CLOSED,
	CELL_DOOR_OPENED  = CELL_DOOR | DOOR_OPENED,

	CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
	CELL_BONUS_BOMBDEC = CELL_BONUS | BONUS_BOMB_NB_DEC,
	CELL_BONUS_BOMBINC = CELL_BONUS | BONUS_BOMB_NB_INC,
	CELL_BONUS_LIFE = CELL_BONUS | BONUS_LIFE
};

enum direction {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
};


enum exit_reason {
	IN_GAME,			// 0
	EXIT_GAME_OVER,		// 1
	EXIT_VICTORY		// 2
};

struct game {
	struct map** maps;       // the game's map - tableau des maps
	short nb_levels;        // nb maps of the game
	short current_level;
	struct player* player;
	struct bomb* bombs;
	struct monster* monsters;
	enum exit_reason exit_reason;
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

struct monster {
	int x, y;
	enum direction current_direction;
	short current_level;
	unsigned int speed;
	struct monster* next;
};

#endif /* INCLUDE_STRUCTURES_H_ */
