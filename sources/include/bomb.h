#ifndef BOMB_H_
#define BOMB_H_
#include <map.h>
#include <game.h>

struct bomb;

struct bomb* bombs_init();

struct bomb* bomb_create(struct game* game, int x, int y);

struct bomb* bombs_add_bomb(struct bomb *bomb, struct game* , int x, int y);

void bomb_update(struct bomb* bombs);

void bomb_display(struct bomb* bombs, struct game* game);

int bomb_get_state(struct bomb* bomb);

int bombs_get_size(struct bomb *bombs);

#endif /* BOMB */
