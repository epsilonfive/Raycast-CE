#ifndef PLAYER_H
#define PLAYER_H

#include "state.h"

#define PLAYER_ROTATION_SPEED 16
#define PLAYER_MOVE_SPEED     4

struct player {
    int x;
    int y;
    int angle;
};

void updatePlayer(struct state *state);
void renderPlayer(struct player *player);

#endif