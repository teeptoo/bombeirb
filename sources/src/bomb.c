#include <SDL/SDL_image.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include <bomb.h>
#include <map.h>
#include <sprite.h>
#include <window.h>
#include <constant.h>
#include <game.h>
#include <player.h>
#include <monster.h>

struct bomb* bombs_init(){
	struct bomb* bombs = NULL;
	return bombs;
}

struct bomb* bomb_create(struct game* game, int x, int y, int range){
	struct bomb* bomb;
	bomb = malloc(sizeof(struct bomb));
	bomb->range = range;
	bomb->x = x;
	bomb->y = y;
	bomb->current_level = game_get_current_level(game);
	bomb->state = 3;
	bomb->mortality = 0;
	short destructed_S = 0;
	short destructed_N = 0;
	short destructed_W = 0;
	short destructed_E = 0;
	bomb->next = NULL;
	map_set_cell_type(game_get_current_map(game), x, y ,CELL_BOMB);
	return bomb;
}

struct bomb* bombs_add_bomb(struct bomb *bombs, struct game* game, int x, int y, int range){
	if (player_get_nb_bomb(game_get_player(game))==0)
		return bombs;

	struct bomb* bomb = bomb_create(game, x, y, range);
	bomb->next = bombs;
	bomb->time_init = SDL_GetTicks() - game->break_time;
	player_dec_nb_bomb(game_get_player(game));
	return bomb;
}

void bomb_update(struct bomb *bombs, struct game* game){
	unsigned int time_live;
	while (bombs != NULL){
			time_live = SDL_GetTicks() - bombs->time_init - game->break_time;
			// time in milliseconds
			if(time_live <= 1000)
				bombs->state=3;
			else if((time_live > 1000) && (time_live <= 2000))
				bombs->state=2;
			else if((time_live > 2000) && (time_live <= 3000))
				bombs->state=1;
			else if((time_live > 3000) && (time_live <= 4000))
				bombs->state=0;
			else if((time_live > 4000) && (time_live <= 5000))
				bombs->state=-1; // bomb during explosion
			else if(time_live > 5000)
				bombs->state=-2; //bomb after explosion
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
			if (temp_bomb->current_level==game_get_current_level(game))
				window_display_image(sprite_get_bomb(state),temp_bomb->x * SIZE_BLOC, temp_bomb->y * SIZE_BLOC);
		}
		temp_bomb=temp_bomb->next;
	}
}
void bomb_explosion_box_type(struct game* game, int x, int y){
	unsigned char type = map_get_full_cell(game_get_current_map(game), x, y);
	switch (type & 0x0f) {
		case BONUS_BOMB_RANGE_DEC:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_RANGEDEC);
			break;
		case BONUS_BOMB_RANGE_INC:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_RANGEINC);
			break;
		case BONUS_BOMB_NB_DEC:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_BOMBDEC);
			break;
		case BONUS_BOMB_NB_INC:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_BOMBINC);
			break;
		case BONUS_LIFE:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_LIFE);
			break;
		case BONUS_MONSTER:
			game_set_monsters(game, monsters_add_monster(game,x,y,game_get_current_level(game)));
			break;
		default:
			map_set_cell_type(game_get_current_map(game), x, y, CELL_EMPTY);
		}
}

short bomb_explosion_aux(struct bomb* bomb, struct game* game, int x, int y, short current_level, short destructed){
	window_display_image(sprite_get_explosion(),x * SIZE_BLOC, y * SIZE_BLOC);
	if ((player_get_x(game_get_player(game))==x) && (player_get_y(game_get_player(game))==y) && bomb->mortality==0){
		player_dec_nb_life(game);
	}
	struct monster* monsters = game_get_monsters(game);
	while(monsters!=NULL){
		if((monsters->x==x) && (monsters->y==y) && (monsters->current_level==current_level)){
			game->monsters=monsters_delete_monster(game_get_monsters(game), monsters);
			map_set_cell_type(game_get_current_map(game),x,y,CELL_EMPTY);
		}
		monsters=monsters->next;
	}
	if ( map_get_cell_type(game_get_current_map(game), x, y) == CELL_BOX && destructed==0){
		bomb_explosion_box_type(game, x, y);
		destructed=1; // bomb a utilisé son quota de destruction de box
	}
	if ( map_get_cell_type(game_get_current_map(game), x, y) == CELL_BONUS && destructed==0){
		map_set_cell_type(game_get_current_map(game), x, y, CELL_EMPTY);
		//destructed=1;
	}

	struct bomb* temp_bomb = game_get_bombs(game);
	while (temp_bomb != bomb){
		if(temp_bomb->x==x && temp_bomb->y==y)
			temp_bomb->time_init = SDL_GetTicks() - 4000;
		temp_bomb=temp_bomb->next;
	}
	return destructed;
}


void bomb_explosion(struct bomb* bomb, struct game* game){
	//SOUTH explosion
	for (int y = bomb->y; y <= (bomb->y+bomb->range); y++) {
		if (map_is_inside(game_get_current_map(game), bomb->x, y)
				&& (map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_EMPTY
						|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BOX
						|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BOMB
						|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_MONSTER
						|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BONUS)
				&& bomb->current_level==game_get_current_level(game)) {
			bomb->destructed_S=bomb_explosion_aux(bomb, game, bomb->x, y, bomb->current_level, bomb->destructed_S);
		}
		else{
			break;
		}
	}
	//NORTH explosion
	for (int y = bomb->y; y >= (bomb->y-bomb->range); y--) {
		if (map_is_inside(game_get_current_map(game), bomb->x, y)
				&& (map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_EMPTY
					|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BOX
					|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BOMB
					|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_MONSTER
					|| map_get_cell_type(game_get_current_map(game), bomb->x, y) == CELL_BONUS)
				&& bomb->current_level==game_get_current_level(game)) {
			bomb->destructed_N = bomb_explosion_aux(bomb, game, bomb->x, y, bomb->current_level, bomb->destructed_N);
		}
		else{
			break;
		}
	}
	//WEST explosion
	for (int x = bomb->x; x >= (bomb->x - bomb->range); x--) {
		if (map_is_inside(game_get_current_map(game), x, bomb->y)
				&& (map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_EMPTY
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BOX
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BOMB
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_MONSTER
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BONUS)
				&& bomb->current_level==game_get_current_level(game)) {
			bomb_explosion_aux(bomb, game, x, bomb->y, bomb->current_level, bomb->destructed_W);
		}
		else{
			break;
		}
	}
	//EST explosion
	for (int x = bomb->x; x <= (bomb->x + bomb->range); x++) {
		if (map_is_inside(game_get_current_map(game), x, bomb->y)
				&& (map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_EMPTY
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BOX
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BOMB
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_MONSTER
					|| map_get_cell_type(game_get_current_map(game), x, bomb->y) == CELL_BONUS)
				&& bomb->current_level==game_get_current_level(game)) {
			bomb_explosion_aux(bomb, game, x, bomb->y, bomb->current_level, bomb->destructed_E);
		}
		else{
			break;
		}
	}
	map_set_cell_type(game_get_map_level(game, bomb->current_level),bomb->x,bomb->y,CELL_EMPTY);
}

void bomb_destruction(struct bomb* bombs, struct game* game){
	// remove the last item from the bombs' list
	  if (bombs->next == NULL){
	    free(bombs);
	    game_set_bomb(game, NULL);
	  }
	  else{
		  struct bomb* temp = bombs;
		  struct bomb* before = bombs;
		  while (temp->next != NULL){
			before = temp;
			temp = temp->next;
		  }
		  before->next = NULL;
		  free(temp);
		  temp = NULL;
		  game_set_bomb(game, bombs);
	  }
	  player_inc_nb_bomb(game_get_player(game));
}

int bomb_get_state(struct bomb* bomb){
	return (bomb->state);
}

int bombs_get_size(struct bomb *bombs){
  if (bombs==NULL)
    return 0;
  int size=0;
  while (bombs!=NULL) {
    size++;
    bombs=bombs->next;
  }
  return size;
}
