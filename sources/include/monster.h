#ifndef MONSTER_H_
#define MONSTER_H_


#include <structures.h>
//#define RAND_MAX 1

struct monster* monsters_init();

struct monster* monster_create(struct game* game, int x, int y);

struct monster* monsters_add_monster(struct game* game, int x, int y);

int monster_move_aux(struct game* game,  int x, int y);

enum direction monster_move_direction();

void monster_move(struct game* game);

void monsters_display(struct monster* monsters, struct game* game);

#endif /* MONSTER */
