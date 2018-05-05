#include <stdlib.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>


struct monster* monsters_init(){
	struct monster* monsters = NULL;
	return monsters;
}

struct monster* monster_create(struct game* game, int x, int y){
	struct monster* monster;
	monster = malloc(sizeof(struct monster));

	monster->x = x;
	monster->y = y;
	monster->current_direction = SOUTH;
	monster->current_level = game_get_current_level(game);
	monster->speed = 1;
	monster->next = NULL;

	map_set_cell_type(game_get_current_map(game), x, y ,CELL_MONSTER);
	return monster;
}

struct monster* monsters_add_monster(struct game* game, int x, int y){
	struct monster* monsters = game_get_monsters(game);//fonction à créer

	struct monster* monster = monster_create(game, x, y);
	monster->next = monsters;
	return monster;
}

int monster_move_aux(struct game* game,  int x, int y) {
	struct map* map = game_get_current_map(game);

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if(map_get_full_cell(map, x, y) == CELL_PRINCESS)
			return 0;
		return 0;
		break;
	case CELL_BOX:
		return 0;
		break;
	case CELL_BONUS:
		return 1;
		break;
	case CELL_KEY:
		return 0;
		break;
	case CELL_MONSTER:
		return 0;
		break;
	case CELL_BOMB:
		return 0;
		break;
	default:
		return 1;
		break;
	}
}

enum direction monster_move_direction(){
	int x = rand()%100;
	if (x < 25)
		return NORTH;
	else if (x < 50)
		return SOUTH;
	else if (x< 75)
		return WEST;
	else
		return EAST;
}

int monster_move(struct game* game) {
	struct map* map = game_get_current_map(game);
	struct monster* monsters = game_get_monsters(game);
	int move = 0;

	while(monsters!=NULL){
		int x = monsters->x;
		int y = monsters->y;
		enum direction direction = monster_move_direction();
		switch (direction) {
		case NORTH:
			if (monster_move_aux(game, x, y - 1)) {
				monsters->y--;
				move = 1;
			}
			break;

		case SOUTH:
			if (monster_move_aux(game, x, y + 1)) {
				monsters->y++;
				move = 1;
			}
			break;

		case WEST:
			if (monster_move_aux(game, x - 1, y)) {
				monsters->x--;
				move = 1;
			}
			break;

		case EAST:
			if (monster_move_aux(game, x + 1, y)) {
				monsters->x++;
				move = 1;
			}
			break;
		default:
			break;
		}
		if (move){
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, monsters->x, monsters->y, CELL_MONSTER);
		}
		monsters=monsters->next;
	}
	return move;
}

void monsters_display(struct monster* monsters, struct game* game) {

	if (SDL_GetTicks()%20<0.1) {
		monster_move(game);
	}
	struct monster* temp_monster = monsters;
	while (temp_monster != NULL){
		if (temp_monster->x==player_get_x(game_get_player(game)) && temp_monster->y==player_get_y(game_get_player(game))) {
			player_dec_nb_life(game);
		}
		window_display_image(sprite_get_monster(temp_monster->current_direction),
				temp_monster->x * SIZE_BLOC, temp_monster->y * SIZE_BLOC);
		temp_monster=temp_monster->next;
	}
}
