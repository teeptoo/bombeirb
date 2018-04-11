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

void display_menu_fixed_elements()
{
	// wallpaper
	window_display_image(sprite_get_menu_wallpaper(), 0, 0);
	// logo
	window_display_image(sprite_get_logo(), MENU_WIDTH/2-(sprite_get_logo()->w)/2, SPLASH_MARGIN);
	// credits
	window_display_image(sprite_get_credits(), MENU_WIDTH/2-(sprite_get_credits()->w)/2, MENU_HEIGHT-SPLASH_MARGIN); // centering credits

}

void launchMenu(void)
{
	int buttons_placement_height[3], x_buttons=MENU_WIDTH/2-(sprite_get_button(0)->w)/2, button_pressed=-1;
	int button_width=sprite_get_button(0)->w, button_height=sprite_get_button(0)->h;
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed, done=0;
	int mouse_x, mouse_y;
	SDL_Event event;

	window_resize(MENU_WIDTH, MENU_HEIGHT);
	while(!done)
	{
		window_clear();
		// fixed elements
		display_menu_fixed_elements();

		// buttons
		buttons_placement_height[0] = 2*SPLASH_MARGIN + sprite_get_logo()->h;
		window_display_image(sprite_get_button(0), x_buttons, buttons_placement_height[0]); // "Reprendre"
		buttons_placement_height[1] = buttons_placement_height[0] + SPLASH_MARGIN/2 + button_height;
		window_display_image(sprite_get_button(2), x_buttons, buttons_placement_height[1]); // "Facile"
		buttons_placement_height[2] = buttons_placement_height[1] + SPLASH_MARGIN/2 + button_height;
		window_display_image(sprite_get_button(4), x_buttons, buttons_placement_height[2]); // "Difficile"

		SDL_GetMouseState(&mouse_x, &mouse_y);
		// Hover button "Reprendre"
		if (mouse_x>=x_buttons && mouse_x<=x_buttons+button_width && mouse_y>=buttons_placement_height[0] && mouse_y<=buttons_placement_height[0]+button_height) {
			window_display_image(sprite_get_button(1), x_buttons, buttons_placement_height[0]);
			button_pressed=0;
		}
		// Hover button "Facile"
		if (mouse_x>=x_buttons && mouse_x<=x_buttons+button_width && mouse_y>=buttons_placement_height[1] && mouse_y<=buttons_placement_height[1]+button_height) {
			button_pressed = 1;
			window_display_image(sprite_get_button(3), x_buttons, buttons_placement_height[1]);
		}
		// Hover button "Difficile"
		if (mouse_x>=x_buttons && mouse_x<=x_buttons+button_width && mouse_y>=buttons_placement_height[2] && mouse_y<=buttons_placement_height[2]+button_height) {
			button_pressed = 2;
			window_display_image(sprite_get_button(5), x_buttons, buttons_placement_height[2]);
		}


		timer = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done=1;
				break;

			}
		}

		window_refresh();

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed);
	}
	launchGame();
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
