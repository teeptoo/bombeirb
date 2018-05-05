/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018
 ******************************************************************************/
#ifndef MENU_H_
#define MENU_H_

// Display menu with mode selection
void launchMenu();

// Launch the given game
void launchGame(struct game* game);

// Display "Game Oveirb" centered and resize window before returning to the menu
void game_over_display();

// Display "Victoire" centered and resize window before returning to the menu
void victory_display();

// Display pause sprite
void break_menu_display(struct game* game);

#endif /* MENU_H_ */
