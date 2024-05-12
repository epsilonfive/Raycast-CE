#include "fast_trig.h"

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <debug.h>

void initializeFastTrig(void) {
    uint32_t *ptr = LUT_LOCATION;
    //initialize sin
    for (int i = 0; i < ANGLE_RESOLUTION; i++)
        *(ptr + i) = (uint32_t) (sin(ANGLE_INCREMENT * i) * TILE_HEIGHT);
    //initialize cosine
    for (int i = 0; i < ANGLE_RESOLUTION; i++)
        *(ptr + i + ANGLE_RESOLUTION) = (uint32_t) (cos(ANGLE_INCREMENT * i) * TILE_WIDTH);
}