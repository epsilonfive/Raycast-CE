#include "fast_trig.h"

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <debug.h>

void initializeFastTrig(void) {
    int *ptr = LUT_LOCATION;
    //initialize sin
    for (int i = 0; i < ANGLE_RESOLUTION; i++)
        *(ptr + i) = (int) (sin(ANGLE_INCREMENT * i) * TILE_HEIGHT);
    //initialize cosine
    for (int i = 0; i < ANGLE_RESOLUTION; i++)
        *(ptr + i + ANGLE_RESOLUTION) = (int) (cos(ANGLE_INCREMENT * i) * TILE_WIDTH);
}

//directly taken from https://en.wikipedia.org/wiki/Fast_inverse_square_root
//originally used in Quake III: Arena, apparently
//uses Newton's method and some horrible tricks to get a rough approximation of an inverse square root
int fastInverseSquareRoot(int number) {
    
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}