/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

#include <structures.h>

// Create a new empty map
struct map* map_new(int width, int height, int starting_x, int starting_y, short * background_RGB);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y); //  4 first bits
enum compose_type map_get_full_cell(struct map* map, int x, int y); // 8 bits

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return the map from specified file
struct map* map_get_from_file(char* file);

// Display the map on the screen
void map_display(struct map* map);

#endif /* MAP_H_ */
