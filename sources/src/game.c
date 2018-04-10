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
#include <constant.h>

struct game {
	struct map** maps;       // the game's map - tableau des maps
	short max_levels;        // nb maps of the game
	short current_level;
	struct player* player;
	struct bomb* bombs;
};

struct game_infos
{
	short max_levels;
	short current_level;
	int current_x;
	int current_y;
	char * map_prefix;
};

struct game_infos* game_get_config_from_file(char * file)
{
	struct game_infos* game_infos = malloc(sizeof(*game_infos));
	game_infos->map_prefix=malloc(MAP_PREFIX_MAX_LENGTH*sizeof(char));
	assert(game_infos);

	// open file
	FILE* game_config_file = NULL;
	game_config_file=fopen(file, "r");
	assert(game_config_file);

	// read nb levels
	assert(fscanf(game_config_file, "%d\n", (int *)&game_infos->max_levels));

	// read current level and pos
	assert(fscanf(game_config_file, "%d:%d,%d\n", (int *)&game_infos->current_level, &game_infos->current_x, &game_infos->current_y));

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
	for (int i = 0; i < game->max_levels; ++i) {
		char map_to_load[50];
		sprintf(map_to_load, "data/maps/map_%i.txt", i);
		game->maps[i] = map_get_from_file(map_to_load);
	}

	// load player infos
	game->player = player_init(4,5);
	// set location of the player
	player_set_position(game->player, game_infos->current_x, game_infos->current_y);

	// set list bombs
	game->bombs=bombs_init(game->maps);

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->max_levels; i++)
		map_free(game->maps[i]);

	free(game);
}

void game_infos_free(struct game_infos* game_infos)
{
	assert(game_infos);
	free(game_infos->map_prefix);
	free(game_infos);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current_level];
}

struct map* game_get_map_level(struct game* game,short level) {
	assert(game);
	return game->maps[level];
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct bomb* game_get_bombs(struct game* game) {
	assert(game);
	return game->bombs;
}

short game_get_current_level(struct game* game) {
	assert(game);
	return game->current_level;
}

void game_set_level(struct game* game, int level) {
	assert(game && level<game->max_levels);
	game->current_level = level;
	window_resize(SIZE_BLOC * map_get_width(game_get_current_map(game)),
		SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
}

void game_set_bomb(struct game * game, struct bomb * bombs){
	assert(game);
	game->bombs = bombs;
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
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_range(game_get_player(game))), x, y);
}

void game_display(struct game* game) {
	assert(game);
	window_clear();

	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	bomb_display(game->bombs, game);

	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	//struct map* map = game_get_current_map(game);
	struct bomb* bombs=game_get_bombs(game);

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
				player_move(game);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(game);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(game);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(game);
				break;
			case SDLK_SPACE:
				game->bombs = bombs_add_bomb(bombs, game, player_get_x(player), player_get_y(player), player_get_range(player));
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
