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
	window_resize(MENU_WIDTH, MENU_HEIGHT);
	window_clear();

	// wallpaper
	window_display_image(sprite_get_menu_wallpaper(), 0, 0);

	// logo
	int x = MENU_WIDTH/2 - (sprite_get_logo()->w)/2; // centering logo
	window_display_image(sprite_get_logo(), x, SPLASH_MARGIN);


	// credits
	x = MENU_WIDTH/2 - (sprite_get_credits()->w)/2;
	window_display_image(sprite_get_credits(), x, MENU_HEIGHT-SPLASH_MARGIN); // centering credits

	window_refresh();
	SDL_Delay(SPLASH_DELAY);
}

void launchGame(void)
{
	struct game_infos* game_infos = game_get_config_from_file("data/games/default.txt");
	struct game* game = game_new(game_infos);
	game_infos_free(game_infos);

	window_resize(SIZE_BLOC * map_get_width(game_get_current_map(game)),
	SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);

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
