/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <structures.h>

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

// Return the list of bombs
struct bomb* game_get_bombs(struct game* game);

// Return the current level
short game_get_current_level(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Return a specified map
struct map* game_get_map_level(struct game* game,short level);

// Change game level
void game_set_level(struct game* game, int level);

// Update the bombs list
void game_set_bomb(struct game * game, struct bomb * bombs);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

#endif /* GAME_H_ */
