#ifndef BOMB_H_
#define BOMB_H_

#include <structures.h>

// Create an empty list of bombs
struct bomb* bombs_init();

// Return a struct bomb according to given parameters
struct bomb* bomb_create(struct game* game, int x, int y, int range);

// Add a bomb to bombs list and decrease player bomb counter
struct bomb* bombs_add_bomb(struct bomb *bomb, struct game* game, int x, int y, int range);

// Update the state of a bomb according to the time since placed
void bomb_update(struct bomb *bombs, struct game* game);

// Display bomb according to his state and call explosion/destructions functions when the state requires it
void bomb_display(struct bomb* bombs, struct game* game);

// Place bonus under destroyed boxes
void bomb_explosion_box_type(struct game* game, int x, int y);

// Destroy boxes arround impact point, according to the range and cell types in the fourth directions
void bomb_explosion(struct bomb* bomb, struct game* game);

// Remove bomb from bombs list and increase player bomb counter
void bomb_destruction(struct bomb* bombs, struct game* game);

// Return the state of a bomb
int bomb_get_state(struct bomb* bomb);

// Return the nomber of bombs inside bombs list
int bombs_get_size(struct bomb *bombs);

#endif /* BOMB */
