/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018
 ******************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <menu.h>
#include <sprite.h>
#include <misc.h>

void splashScreen(void)
{
	window_resize(SPLASH_WIDTH, SPLASH_HEIGHT);
	window_clear();

	// logo
	int x = SPLASH_WIDTH/2 - (sprite_get_logo()->w)/2; // centering logo
	window_display_image(sprite_get_logo(), x, SPLASH_MARGIN);

	// map level number
	x = SPLASH_WIDTH/2 - (sprite_get_number(0)->w)/2; // centering map number in both direction
	int y = SPLASH_HEIGHT/2 - (sprite_get_number(0)->h)/2;
	window_display_image(sprite_get_number(0), x,y);

	// credits
	x = SPLASH_WIDTH/2 - (sprite_get_credits()->w)/2;
	window_display_image(sprite_get_credits(), x, SPLASH_HEIGHT-SPLASH_MARGIN); // centering credits

	window_refresh();
	SDL_Delay(3000);
}

void launchGame(void)
{
	struct game_infos* game_infos = game_get_config_from_file("data/games/default.txt");
	struct game* game = game_new(game_infos);
	game_infos_free(game_infos);

	window_resize(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT); // TO DEBUG -> taille non prise en compte

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	game_free(game);
}
