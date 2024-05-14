#ifndef FAST_TRIG_H
#define FAST_TRIG_H

#include "map.h"

#define LUT_LOCATION ((int *) 0x0D031F6)
#define TAU ((double) 3.141592653 * 2) //2pi
#define ANGLE_RESOLUTION 1024 //what represents tau in our new system?
#define ANGLE_INCREMENT (TAU / ANGLE_RESOLUTION)

//the actual operations
#define FAST_SIN(angle) *(LUT_LOCATION + angle)
#define FAST_COS(angle) *(LUT_LOCATION + ANGLE_RESOLUTION + angle)

void initializeFastTrig(void);

#endif