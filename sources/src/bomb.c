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

struct bomb {
	int range;
	int x, y;
	int map; //pas sur du type
	int state;
	struct bomb *next;
};

struct bomb *create_bombs(int x, int y, int map, int state){
	struct bomb *bomb;
	bomb = malloc(sizeof(*bomb));
	// besoin de l'allocation dynamique ?
	bomb->x = x;
	bomb->y = y;
	bomb->map = map;
	bomb->state = state;
	bomb->next = NULL;

	return bomb;
}

int sizeList(struct bomb *list)
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

struct bomb *add_bomb(struct bomb *list,struct bomb *bomb)
{
    bomb->next=list;
    return bomb;
}
