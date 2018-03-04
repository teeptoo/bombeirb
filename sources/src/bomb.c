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


struct bomb* create_bombs(int x, int y, struct map* map){
	struct bomb *bomb;
	bomb = malloc(sizeof(*bomb));
	// besoin de l'allocation dynamique pour chaque élément?
	bomb->x = x;
	bomb->y = y;
	bomb->map = map;
	bomb->state = 1;
	bomb->next = NULL;
	map_set_cell_type(map, x, y, CELL_BOMB);

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
int bomb_get_state(struct bomb* bomb){
	return (bomb->state);
}

void bomb_add(struct bomb *list,struct bomb *bomb)
{
    bomb->next=list;
    *list=*bomb;
}

struct bomb* bomb_init(struct map* map){
	struct bomb* bomb = malloc(sizeof(*bomb));
		if (!bomb)
			error("Memory error");

		bomb->x = 5;
		bomb->y = 0;
		bomb->range=1;
		bomb->map=map;
		bomb->state=1;
		bomb->next=NULL;
		//map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);

		return bomb;
}

void bomb_placed(int x, int y, struct map* map, struct bomb* list){
	assert(list);
	struct bomb* bomb;
	bomb=create_bombs(x, y, map);
	//on considère que le 1er maillon de la liste existe déjà
	bomb_add(list,bomb);
}


void bomb_display(struct bomb* bombs) {
	assert(bombs);
	int state=bomb_get_state(bombs);
	while (bombs != NULL){
		window_display_image(sprite_get_bomb(state),bombs->x * SIZE_BLOC, bombs->y * SIZE_BLOC);
		bombs=bombs->next;
	}
}
