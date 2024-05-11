#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define MAP_WIDTH      10
#define MAP_HEIGHT     10
#define TILE_WIDTH_PX  8
#define TILE_HEIGHT_PX 8

#define MINIMAP_X      5
#define MINIMAP_Y      5

struct map {
    uint8_t data[MAP_WIDTH][MAP_HEIGHT];
};

void initializeMap(struct map *map);
void renderMap(struct map *map);

#endif