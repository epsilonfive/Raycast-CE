#ifndef STATE_H
#define STATE_H

#include "map.h"
#include "player.h"
#include "input.h"

struct state {
    struct map *map;
    struct player *player;
    struct input *input;
};

#endif