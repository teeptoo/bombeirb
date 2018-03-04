
#include <map.h>

struct bomb;

struct bomb* create_bombs(int x, int y, struct map* map);

int bomb_get_size_list(struct bomb *list);

void bomb_add(struct bomb *list,struct bomb *bomb);

//struct bomb* get_list_bomb(game);//fonction non codé

void bomb_placed(int x, int y, struct map* map, struct bomb* list);

void bomb_display(struct bomb* bomb);
