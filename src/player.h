#ifndef PLAYER_H
#define PLAYER_H

#include "state.h"

#define PLAYER_ROTATION_SPEED 0.1
#define PLAYER_MOVE_SPEED     3

struct player {
    float x;
    float y;
    float angle;
};

void updatePlayer(struct state *state);
void renderPlayer(struct player *player);

#endif