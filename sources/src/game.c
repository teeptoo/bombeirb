/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>

struct game {
	struct map** maps;       // the game's map
	short max_levels;        // nb maps of the game
	short current_level;
	struct player* player;
};

struct game_infos
{
	short max_levels;
	short current_level;
	int current_x;
	int current_y;
	char * map_prefix[];
};

struct game_infos* game_get_config_from_file(char * file)
{
	struct game_infos* game_infos = malloc(sizeof(*game_infos));
	assert(game_infos);

	// open file
	FILE* game_config_file = NULL;
	game_config_file=fopen(file, "r");
	assert(game_config_file);

	// read nb levels
	assert(fscanf(game_config_file, "%d\n", (int *)&game_infos->max_levels));

	// read current level and pos
	assert(fscanf(game_config_file, "%i:%d,%d\n", (int *)&game_infos->current_level, &game_infos->current_x, &game_infos->current_y));

	// read map prefix
	assert(fscanf(game_config_file, "%s", (char *)game_infos->map_prefix));

	fclose(game_config_file);
	return game_infos;
}

struct game* game_new(struct game_infos* game_infos) {
	struct game* game = malloc(sizeof(*game));
	assert(game);

	game->maps = malloc(sizeof(struct game));
	// load game infos
	game->max_levels = game_infos->max_levels;
	game->current_level = game_infos->current_level;
	// load maps
	game->maps[0] = map_get_from_file("data/maps/map_0.txt");

	// load player infos
	game->player = player_init(1);
	// set location of the player
	player_set_position(game->player, game_infos->current_x, game_infos->current_y);

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->max_levels; i++)
		map_free(game->maps[i]);

	free(game);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current_level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(2), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(1), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();

	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);

	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game

	return 0;
}
