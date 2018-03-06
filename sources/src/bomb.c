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
  short current_level;
  int state;
  struct bomb *next;
};

struct bomb* bombs_init(){
	struct bomb* bombs = NULL;
	return bombs;
}

struct bomb* bomb_create(struct game* game, int x, int y){
	struct bomb* bomb;
	bomb = malloc(sizeof(struct bomb));
	bomb->range = 1;
	bomb->x = x;
	bomb->y = y;
	bomb->current_level = game_get_current_level(game);
	bomb->state = 3;
	bomb->next = NULL;
	map_set_cell_type(game_get_current_map(game), x, y ,CELL_BOMB);

	return bomb;
}



struct bomb* bombs_add_bomb(struct bomb *bombs, struct game* game, int x, int y){
	if (player_get_nb_bomb(game_get_player(game))==0){
		return bombs;
	}
	struct bomb* bomb = bomb_create(game, x, y);
	bomb->next = bombs;
	player_dec_nb_bomb(game_get_player(game));
	return bomb;
}


void bomb_display(struct bomb* bombs) {
	int state;
	while (bombs != NULL){
		state=bomb_get_state(bombs);
		window_display_image(sprite_get_bomb(state),bombs->x * SIZE_BLOC, bombs->y * SIZE_BLOC);
		bombs=bombs->next;
	}
}

void explosion_display(struct bomb* bomb){
	window_display_image(sprite_get_explosion(),bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
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
