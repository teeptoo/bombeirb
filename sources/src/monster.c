#include <stdlib.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <player.h>

int monster_get_size(struct monster* monsters){
	int i = 0;
	while(monsters!=NULL){
		i++;
		monsters=monsters->next;
	}
	return i;
}

void monsters_init(struct game* game){
	game->monsters = NULL;
	// détection des monstres sur les cartes
	for (int i = 0; i < game->nb_levels; i++)
		for (int x = 0; x < game->maps[i]->width; x++)
			for (int y = 0; y < game->maps[i]->height; y++)
				if (map_get_cell_type(game->maps[i], x, y)==CELL_MONSTER){
					game_set_monsters(game, monsters_add_monster(game, x, y, i));
				}
}

struct monster* monster_create(struct game* game, int x, int y, short map_level){
	struct monster* monster;
	monster = malloc(sizeof(struct monster));
	monster->x = x;
	monster->y = y;
	monster->current_direction = SOUTH;
	monster->current_level = map_level;
	monster->time_speed = SDL_GetTicks();
	monster->time_init = SDL_GetTicks();
	monster->next = NULL;
	map_set_cell_type(game_get_map_level(game, map_level), x, y ,CELL_MONSTER);
	return monster;
}

struct monster* monsters_add_monster(struct game* game, int x, int y, short map_level){
	struct monster* monsters = game_get_monsters(game);
	struct monster* monster = monster_create(game, x, y, map_level);
	monster->next = monsters;
	return monster;
}

int monster_move_aux(struct game* game,  int x, int y, struct map* map) {
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
	case CELL_DOOR:
		return 0;
	default:
		return 1;
		break;
	}
}

int monster_move_aux_door(struct game* game,  int x, int y, struct map* map){
	if (map_is_inside(map, x, y)==0)
		return 1;
	if (map_get_cell_type(map, x, y)==CELL_DOOR)
		return 0;
	return 1;
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

void monster_move(struct game* game, struct monster* monsters) {
	struct map* map = game_get_map_level(game, monsters->current_level);
	int move = 0, rand_counter=0;
	enum direction direction;

	int x = monsters->x;
	int y = monsters->y;
	while(!move && rand_counter < 10)
	{
		direction = monster_move_direction();
		switch (direction) {
		case NORTH:
			if (monster_move_aux(game, x, y - 1, map)
					&& monster_move_aux_door(game, x, y-2, map)
					&& monster_move_aux_door(game, x-1, y-1 , map)
					&& monster_move_aux_door(game, x+1, y-1, map)) {
				monsters->current_direction=NORTH;
				monsters->y--;
				move = 1;
			}
			break;
		case SOUTH:
			if (monster_move_aux(game, x, y + 1, map)
					&& monster_move_aux_door(game, x, y + 2, map)
					&& monster_move_aux_door(game, x-1, y+1, map)
					&& monster_move_aux_door(game, x+1, y+1, map)) {
				monsters->current_direction=SOUTH;
				monsters->y++;
				move = 1;
			}
			break;
		case WEST:
			if (monster_move_aux(game, x - 1, y, map)
					&& monster_move_aux_door(game, x-2, y, map)
					&& monster_move_aux_door(game, x-1, y-1, map)
					&& monster_move_aux_door(game, x-1, y+1, map)) {
				monsters->current_direction=WEST;
				monsters->x--;
				move = 1;
			}
			break;
		case EAST:
			if (monster_move_aux(game, x + 1, y, map)
					&& monster_move_aux_door(game, x + 2, y, map)
					&& monster_move_aux_door(game, x+1, y-1, map)
					&& monster_move_aux_door(game, x+1, y+1, map)) {
				monsters->current_direction=EAST;
				monsters->x++;
				move = 1;
			}
			break;
		default:
			break;
		}
		rand_counter++;
	}
	if(rand_counter < 10)
	{
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monsters->x, monsters->y, CELL_MONSTER);
		monsters->time_speed=SDL_GetTicks(); // remise à 0 du compteur pour la vietsse quand le monstre a bougé
	}
}

struct monster *monsters_delete_monster(struct monster *monsters,struct monster *monster){
	unsigned int time = SDL_GetTicks() - monster->time_init;
	if(time<1000)	// le monstre est immunisé pendant sa premiere seconde de vie
		return monsters;
	if (monster_get_size(monsters)==1){
		if (monsters->x==monster->x && monsters->y==monster->y && monsters->current_level==monster->current_level)
		  return NULL;
		else
		  return monsters;
	}
	else{
	  if (monsters->x==monster->x && monsters->y==monster->y && monsters->current_level==monster->current_level)
		return monsters=monsters->next;
	  if (monsters->next->x==monster->x && monsters->next->y==monster->y && monsters->next->current_level==monster->current_level){
		  monsters->next = NULL;
		  return monsters;
	  }
	  while (monsters!=NULL){
		if (monster->x==monsters->next->x && monster->y==monsters->next->y && monster->current_level==monsters->next->current_level)
		  monsters->next=monsters->next->next;
		else
			monsters=monsters->next;
	  }
	}
	return monsters;
}

void monsters_display(struct monster* monsters, struct game* game) {
	struct monster* temp_monster = monsters;
	while (temp_monster != NULL){
		// la vitesse de délpalcement est fonction du level
		if (SDL_GetTicks()-temp_monster->time_speed>(2500-game_get_current_level(game)*1000)) {
				monster_move(game, temp_monster);
				temp_monster->time_speed = SDL_GetTicks();
		}
		if (temp_monster->x==player_get_x(game_get_player(game)) && temp_monster->y==player_get_y(game_get_player(game)) && temp_monster->current_level==game_get_current_level(game))
			player_dec_nb_life(game);
		if (temp_monster->current_level == game_get_current_level(game))
			window_display_image(sprite_get_monster(temp_monster->current_direction),temp_monster->x * SIZE_BLOC, temp_monster->y * SIZE_BLOC);
		temp_monster=temp_monster->next;
	}
}
