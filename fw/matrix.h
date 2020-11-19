// By Marc MERLIN <marc_soft@merlins.org>
// License: Apache v2.0
//

#ifndef matrix_h
#define matrix_h

//#include "neomatrix_config.h"
//#include "matrix.h"
//#include "Effects.h"

#include "effects/Boid.h"
#include "effects/Attractor.h"
#include "effects/Geometry.h"


uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t speed = 255;

float matrix_gamma = 1; // higher number is darker, needed for Neomatrix more than SmartMatrix


#endif
