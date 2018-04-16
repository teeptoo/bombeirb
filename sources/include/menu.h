/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018
 ******************************************************************************/
#ifndef MENU_H_
#define MENU_H_

// Display menu with mode selection
void launchMenu();

// Launch depending on a configuration file
void launchGame(char* config_file);

// Display "Game Oveirb" centered and resize window before returning to the menu
void game_over_display();

// Display "Victoire" centered and resize window before returning to the menu
void victory_display();

#endif /* MENU_H_ */
