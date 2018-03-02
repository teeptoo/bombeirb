


struct bomb;

struct bomb *create_bombs(int x, int y, int map, int state);

int sizeList(struct bomb *list);

struct bomb *add_bomb(struct bomb *list,struct bomb *bomb);
