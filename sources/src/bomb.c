#include <SDL/SDL_image.h>
#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <bomb.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>


struct bomb{
  int range;
  int x, y;
  struct map* map;
  int state;
  struct bomb *next;
};


struct bomb *create_bombs(int x, int y, struct map* map){
	struct bomb *bomb;
	bomb = malloc(sizeof(*bomb));
	// besoin de l'allocation dynamique pour chaque élément?
	bomb->x = x;
	bomb->y = y;
	bomb->map = map;
	bomb->state = 0;
	bomb->next = NULL;

	return bomb;
}

int bomb_get_size_list(struct bomb *list)
{
  if (list==NULL)
  {
    return 0;
  }
  int size=0;
  while (list!=NULL) {
    size++;
    list=list->next;
  }
  return size;
}

struct bomb *bomb_add(struct bomb *list,struct bomb *bomb)
{
    bomb->next=list;
    return bomb;
}

struct bomb *bomb_placed(int x, int y, struct map* map,, struct bomb *list){
	struct bomb *bomb;
	bomb=create_bombs(x, y, map);
	if (bomb_get_size_liste(list)==0){
		return(bomb);
	}
	return(bomb_add(list,bomb));
}


void bomb_display(struct bomb* bomb) {
	assert(bomb);
	window_display_image(sprite_get_bomb(),bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
}
