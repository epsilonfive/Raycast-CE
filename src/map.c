#include "map.h"

#include <string.h>
#include <graphx.h>

void initializeMap(struct map *map) {
    static uint8_t data[MAP_HEIGHT][MAP_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    //initialize the map data
    memcpy(&map->data, data, MAP_WIDTH * MAP_HEIGHT);
}

void renderMap(struct map *map) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map->data[j][i]) gfx_SetColor(222);
            else gfx_SetColor(255);
            gfx_FillRectangle(MINIMAP_X + i * MINIMAP_TILE_WIDTH_PX,
                MINIMAP_X + j * MINIMAP_TILE_HEIGHT_PX, MINIMAP_TILE_WIDTH_PX, MINIMAP_TILE_HEIGHT_PX); 
        }
    }
    //draw the outline of the map as well
    //gfx_SetColor(0);
    //gfx_Rectangle(MINIMAP_X - 1, MINIMAP_Y - 1, MAP_WIDTH * MINIMAP_TILE_WIDTH_PX + 2, MAP_HEIGHT * MINIMAP_TILE_HEIGHT_PX + 2);
}