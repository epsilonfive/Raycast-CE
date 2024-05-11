#include "player.h"

#include <graphx.h>
#include <math.h>

#include "state.h"

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
    if (getKeyState(state->input, KEY_UP)) {
        target_x += PLAYER_MOVE_SPEED * cos(player->angle);
        target_y += PLAYER_MOVE_SPEED * sin(player->angle);
    }
    //bounds checking
    if (target_x < 0) target_x = 0;
    if (target_y < 0) target_y = 0;
    if (target_x >= MAP_WIDTH * TILE_WIDTH_PX) target_x = MAP_WIDTH * TILE_WIDTH_PX;
    if (target_y >= MAP_HEIGHT * TILE_HEIGHT_PX) target_y = MAP_HEIGHT * TILE_HEIGHT_PX;
    //move the player if valid
    if (!state->map->data[target_y / TILE_HEIGHT_PX][target_x / TILE_WIDTH_PX]) {
        player->x = target_x;
        player->y = target_y;
    }
}

void renderPlayer(struct player *player) {
    player->x += MINIMAP_X;
    player->y += MINIMAP_Y;
    gfx_SetColor(224);
    gfx_FillCircle(player->x, player->y, 3);
    gfx_Line(player->x, player->y, player->x + 10 * cos(player->angle), 
        player->y + 10 * sin(player->angle));
    player->x -= MINIMAP_X;
    player->y -= MINIMAP_Y;
}