#ifndef MAP_H
#define MAP_H

#include <stdint.h>

//map width and height in tiles
#define MAP_WIDTH              10
#define MAP_HEIGHT             MAP_WIDTH

//tile width and height in our special units
#define TILE_WIDTH             256
#define TILE_HEIGHT            TILE_WIDTH

//minimap
#define MINIMAP_TILE_WIDTH_PX  8
#define MINIMAP_TILE_HEIGHT_PX MINIMAP_TILE_WIDTH_PX
#define MINIMAP_X              5
#define MINIMAP_Y              5

struct map {
    uint8_t data[MAP_WIDTH][MAP_HEIGHT];
};

void initializeMap(struct map *map);
void renderMap(struct map *map);

#endif