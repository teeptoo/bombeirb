#ifndef BOMB_H_
#define BOMB_H_
#include <map.h>

struct bomb;

struct bomb* bomb_create(struct map* map, int x, int y);

void bombs_add_bomb(struct bomb *bombs, struct bomb *bomb);

struct bomb* bombs_init();

void bomb_display(struct bomb* bombs);

int bomb_get_state(struct bomb* bomb);

int bombs_get_size(struct bomb *bombs);

#endif /* BOMB */
