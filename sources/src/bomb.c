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
  unsigned int time_init;
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
	bomb->range = 2;
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
	bomb->time_init = SDL_GetTicks();
	player_dec_nb_bomb(game_get_player(game));
	return bomb;
}

void bomb_update(struct bomb *bombs, struct game* game){
	unsigned int time_live;
	while (bombs != NULL){
			time_live = SDL_GetTicks() - bombs->time_init;
			// time in milliseconds
			if(time_live < 1000){
				bombs->state=3;
			}
			else if((time_live < 2000) && (time_live > 1000)){
				bombs->state=2;
			}
			else if((time_live < 3000) && (time_live > 2000)){
				bombs->state=1;
			}
			else if((time_live < 4000) && (time_live > 3000)){
				bombs->state=0;
			}
			else if((time_live > 4000) && (time_live < 5000)){
				bombs->state=-1; //bomb en train d'exploser
			}
			if(time_live > 5000){
				bombs->state=-2; //bomb après explosion
			}
			bombs=bombs->next;
		}
}


void bomb_display(struct bomb* bombs, struct game* game) {
	int state;
	bomb_update(bombs, game);
	struct bomb* temp_bomb = bombs;

	while (temp_bomb != NULL){
		state=bomb_get_state(temp_bomb);
		switch (state){
		case -1:
			bomb_explosion(temp_bomb, game);
			break;
		case -2:
			bomb_destruction(bombs, game);
			break;
		default:
			window_display_image(sprite_get_bomb(state),temp_bomb->x * SIZE_BLOC, temp_bomb->y * SIZE_BLOC);
		}
		temp_bomb=temp_bomb->next;
	}
}

void bomb_explosion(struct bomb* bomb, struct game* game){
	//SOUTH explosion
	for (int y = bomb->y; y <= (bomb->y+bomb->range); y++) {
		if (map_is_inside(game_get_current_map(game), bomb->x, y) && map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_EMPTY) {
			window_display_image(sprite_get_explosion(),bomb->x * SIZE_BLOC, y * SIZE_BLOC);
		}
		else{
			break;
		}
	}
	//NORTH explosion
	for (int y = bomb->y; y >= (bomb->y-bomb->range); y--) {
		if (map_is_inside(game_get_current_map(game), bomb->x, y) && (map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_EMPTY)) {
			window_display_image(sprite_get_explosion(),bomb->x * SIZE_BLOC, y * SIZE_BLOC);
		}
		else{
			break;
		}
	}
	//WEST explosion
	for (int x = bomb->x; x >= (bomb->x - bomb->range); x--) {
		if (map_is_inside(game_get_current_map(game), x, bomb->y) && (map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_EMPTY)) {
			window_display_image(sprite_get_explosion(),x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		}
		else{
			break;
		}
	}
	//EST explosion
	for (int x = bomb->x; x <= (bomb->x + bomb->range); x++) {
		if (map_is_inside(game_get_current_map(game), x, bomb->y) && (map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_EMPTY)) {
			window_display_image(sprite_get_explosion(),x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		}
		else{
			break;
		}
	}

	map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y,CELL_EMPTY);
}

void bomb_destruction(struct bomb* bombs, struct game* game){
	// remove the last item from the bombs' list
	  if (bombs->next == NULL)
	  {
	    free(bombs);
	    game_set_bomb(game, NULL);
	  }
	  else
	  {

		  struct bomb* temp = bombs;
		  struct bomb* before = bombs;

		  while (temp->next != NULL)
		  {
			before = temp;
			temp = temp->next;
		  }
		  before->next = NULL;
		  free(temp);
		  temp = NULL;
		  game_set_bomb(game, bombs);
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
