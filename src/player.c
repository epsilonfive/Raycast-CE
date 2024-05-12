#include "player.h"

#include <graphx.h>
#include <math.h>

#include "state.h"
#include "fast_trig.h"

void updatePlayer(struct state *state) {
    struct player *player = state->player;
    bool rotated = false;
    int target_x = player->x;
    int target_y = player->y;
    if (getKeyState(state->input, KEY_LEFT)) {
        player->angle -= PLAYER_ROTATION_SPEED;
        rotated = true;
    }
    if (getKeyState(state->input, KEY_RIGHT)) {
        player->angle += PLAYER_ROTATION_SPEED;
        rotated = true;
    }
    player->angle %= ANGLE_RESOLUTION;
    if (getKeyState(state->input, KEY_UP)) {
        target_x += FAST_COS(player->angle) / PLAYER_MOVE_SPEED;
        target_y += FAST_SIN(player->angle) / PLAYER_MOVE_SPEED;
    }
    //bounds checking
    if (target_x < 0) target_x = 0;
    if (target_y < 0) target_y = 0;
    if (target_x >= MAP_WIDTH * TILE_WIDTH) target_x = MAP_WIDTH * TILE_WIDTH;
    if (target_y >= MAP_HEIGHT * TILE_HEIGHT) target_y = MAP_HEIGHT * TILE_HEIGHT;
    //move the player if valid
    if (!state->map->data[target_y / TILE_HEIGHT][target_x / TILE_WIDTH]) {
        player->x = target_x;
        player->y = target_y;
    }
}

void renderPlayer(struct player *player) {
    player->x >>= 5;
    player->y >>= 5;
    player->x += MINIMAP_X;
    player->y += MINIMAP_Y;
    gfx_SetColor(224);
    gfx_FillCircle(player->x, player->y, 3);
    gfx_Line(player->x, player->y, player->x + (FAST_COS(player->angle) >> 4), 
        player->y + (FAST_SIN(player->angle) >> 4));
    player->x -= MINIMAP_X;
    player->y -= MINIMAP_Y;
    player->x <<= 5;
    player->y <<= 5;
}