/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>

struct player* player_init(int bomb_number, short life_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;
	player->nb_life = life_number;
	player->range = 2;
	player->nb_keys = 1;

	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_range(struct player* player) {
	assert(player != NULL);
	return player->range;
}

void player_inc_range(struct player* player) {
	assert(player);
	player->range += 1;
}

void player_dec_range(struct player* player) {
	assert(player);
	player->range -= 1;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs -= 1;
}

short player_get_nb_life(struct player* player) {
	assert(player);
	return player->nb_life;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	player->nb_life -= 1;
}

short player_get_nb_keys(struct player* player) {
	assert(player);
	return player->nb_keys;
}

void player_inc_nb_keys(struct player* player) {
	assert(player);
	player->nb_keys += 1;
}

void player_dec_nb_keys(struct player* player) {
	assert(player);
	player->nb_keys -= 1;
}

void player_move_bonus(struct game* game, int x, int y){
	struct map* map = game_get_current_map(game);
	struct player* player = game_get_player(game);

	switch (map_get_full_cell(map, x, y)){
	case (CELL_BONUS_RANGEDEC):
		player_dec_range(player);
		break;
	case CELL_BONUS_RANGEINC:
		player_inc_range(player);
		break;
	case CELL_BONUS_BOMBDEC:
		player_dec_nb_bomb(player);
		break;
	case CELL_BONUS_BOMBINC:
		player_inc_nb_bomb(player);
		break;
	case CELL_BONUS_LIFE:
		player_inc_nb_life(player);
		break;

	}
}

static int player_move_aux(struct game* game, int x, int y) {
	struct map* map = game_get_current_map(game);

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_BONUS:
		player_move_bonus(game, x, y);
		return 1;
		break;

	case CELL_MONSTER:
		break;

	case CELL_BOMB:
		return 0;
		break;
	case CELL_DOOR:
		switch(map_get_full_cell(map, x, y) & 0x31 ) {
		case CELL_DOOR_OPENED:
			game_set_level(game, (map_get_full_cell(map, x, y) & 0x0E) >> 1);
			return 0;
			break;
		case CELL_DOOR_CLOSED:
			return 0;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct game* game) {
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);

	int x = player->x;
	int y = player->y;
	int move = 0;
	int box_movement_x = x;
	int box_movement_y = y;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(game, x, y - 1)) {
			player->y--;
			box_movement_y = player->y - 1;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(game, x, y + 1)) {
			player->y++;
			box_movement_y = player->y + 1;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(game, x - 1, y)) {
			player->x--;
			box_movement_x = player->x - 1;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(game, x + 1, y)) {
			player->x++;
			box_movement_x = player->x + 1;
			move = 1;
		}
		break;
	}
	if (move)
	{
		if (map_get_cell_type(map, player->x, player->y) == CELL_BOX)
		{
			if ((!map_is_inside(map,box_movement_x, box_movement_y))||((map_get_cell_type(map, box_movement_x, box_movement_y) != CELL_EMPTY)))
			{// check box outside map || check to not push box on another box
					player->x=x;
					player->y=y;
					move=0;
					return move;
			}
			map_set_cell_type(map, box_movement_x, box_movement_y, CELL_BOX);
		}
		map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
