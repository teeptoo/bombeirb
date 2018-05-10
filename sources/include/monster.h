#ifndef MONSTER_H_
#define MONSTER_H_


#include <structures.h>
//#define RAND_MAX 1

void monsters_init(struct game* game);

struct monster* monster_create(struct game* game, int x, int y, short map_level);

struct monster* monsters_add_monster(struct game* game, int x, int y, short map_level);

int monster_move_aux(struct game* game,  int x, int y, struct map* map);

enum direction monster_move_direction();

void monster_move(struct game* game, struct monster* monsters);

struct monster *monsters_delete_monster(struct monster *monsters,struct monster *monster);

void monsters_display(struct monster* monsters, struct game* game);

#endif /* MONSTER */
