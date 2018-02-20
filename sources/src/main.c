/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <menu.h>


int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	launchGame();

	SDL_Quit();

	return EXIT_SUCCESS;
}
