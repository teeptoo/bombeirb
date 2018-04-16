/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>



#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height, int starting_x, int starting_y)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;
	map->starting_x = starting_x;
	map->starting_y = starting_y;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if( (x >= 0) && (x < map->width) && (y >= 0) && (y < map->height) )
		return 1;
	return 0;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_full_cell(struct map* map, int x, int y)
{
	assert (map && map_is_inside(map, x, y));
	return map-> grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	// bonnus des vie non gérer (pas d'image assocsier)
	/*case BONUS_LIFE:
		window_display_image(sprite_get_bonus(à compléter), x, y);
		break;*/
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
		break;
	}
}

void display_doors(struct map* map, int x, int y, unsigned char type)
{
	switch (type & 0x01)
	{
	case DOOR_CLOSED:
		window_display_image(sprite_get_door_closed(), x, y);
		break;
	case DOOR_OPENED:
		window_display_image(sprite_get_door_opened(), x, y);
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
	    	display_doors(map, x, y, type);
	    	break;
	    }
	  }
	}
}

struct map* map_get_from_file(char* file)
{
	int map_width=0, map_height=0, map_starting_x=0, map_starting_y=0, pos_line, pos_elt;
	char *line_temp=NULL, *token=NULL;

	// open file
	FILE* map_file = NULL;
	map_file=fopen(file, "r");
	assert(map_file);

	// get map size
	assert(fscanf(map_file, "%d x %d\n", &map_width, &map_height));

	// get map starting point
	assert(fscanf(map_file, "%d ; %d\n", &map_starting_x, &map_starting_y));

	// init pointers
	line_temp = malloc(3 * (map_width+1) * sizeof(char)); // 2 digits and 1 blank max per grid element + 3*1 for delimiters (\n and \0)
	assert(line_temp);

	// init map
	struct map* map = map_new(map_width, map_height, map_starting_x, map_starting_y);
	assert(map);

	// get map's content
	for (pos_line=0; pos_line<map->height; pos_line++)
	{
		// for each line, read it from file, then split, then filling map grid
		assert(fgets(line_temp, 3*(map->width+1)*sizeof(char), map_file)); // current line
		token = strtok(line_temp, " "); // split with spaces
		for(pos_elt=0; pos_elt<map->width; pos_elt++)
		{
			map->grid[pos_line*map->width + pos_elt]=(unsigned char)atoi(token); // filling map grip
			token=strtok(NULL, " "); // grab next occurrence
		} // END for(pos_elt=0; pos_elt<map_width; pos_elt++)
	} // END for (pos_line=O; pos_line<map_height; pos_line++)

	fclose(map_file);
	free(line_temp);
	return map;
}

