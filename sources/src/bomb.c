#include <SDL/SDL_image.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include <bomb.h>
#include <map.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb {
  int range;
  int x, y;
  int map_number;
  int state;
  struct bomb *next;
};


struct bomb* bomb_create(struct map* map, int x, int y){
	struct bomb *bomb;
	bomb = malloc(sizeof(*bomb));
	bomb->x = x;
	bomb->y = y;
	bomb->state = 3;
	bomb->next = NULL;

	 // map_set_cell_type(map, x, y CELL_BOMB);

	return bomb;
}



void bombs_add_bomb(struct bomb *bombs, struct bomb *bomb)
{

}

struct bomb* bombs_init(){
	struct bomb* bombs = NULL;
	return bombs;
}

void bomb_display(struct bomb* bombs) {
	int state;
	while (bombs != NULL){
		state=bomb_get_state(bombs);
		window_display_image(sprite_get_bomb(state),bombs->x * SIZE_BLOC, bombs->y * SIZE_BLOC);
		bombs=bombs->next;
	}
}

int bomb_get_state(struct bomb* bomb){
	return (bomb->state);
}

int bombs_get_size(struct bomb *bombs)
{
  if (bombs==NULL)
  {
    return 0;
  }
  int size=0;
  while (bombs!=NULL) {
    size++;
    bombs=bombs->next;
  }
  return size;
}
