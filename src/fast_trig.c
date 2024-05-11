#include "fast_trig.h"

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <debug.h>

void initializeFastTrig(void) {
    uint32_t *ptr = LUT_LOCATION;
    //initialize sin
    for (int i = 0; i < ANGLE_RESOLUTION; i++) {
        *(ptr + i) = (uint32_t) (sin(TAU / ANGLE_RESOLUTION) * TILE_WIDTH);
        dbg_sprintf(dbgout, "sin(%d) is equal to %d", i, *(ptr + i));
        ptr++;
    }
    //initialize cosine
    for (int i = 0; i < ANGLE_RESOLUTION; i++) {
        *(ptr + i) = (uint32_t) (cos(TAU / ANGLE_RESOLUTION) * TILE_WIDTH);
        ptr++;
    }
}