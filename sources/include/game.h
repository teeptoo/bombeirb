/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <bomb.h>

// Abstract data type
struct game;
struct game_infos;

// Get infos from config file
struct game_infos* game_get_config_from_file(char * file);

// Create a new game
struct game* game_new(struct game_infos* game_infos);

// Free a game
void game_free(struct game* game);

// Free game infos
void game_infos_free(struct game_infos* game_infos);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Return the current level
short game_get_current_level(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

#endif /* GAME_H_ */
