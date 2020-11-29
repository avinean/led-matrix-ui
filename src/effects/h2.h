#ifndef _H2_H_
#define _H2_H_

#ifdef _SEPARATE_
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>


#define MX_WIDTH 16
#define MATRIX_WIDTH MX_WIDTH
#define MX_HEIGHT 16
#define MATRIX_HEIGHT MX_HEIGHT

#define NUM_LEDS (MX_WIDTH*MX_HEIGHT)
#define NUMMATRIX NUM_LEDS

const int MATRIX_CENTER_X = MATRIX_WIDTH / 2;
const int MATRIX_CENTER_Y = MATRIX_HEIGHT / 2;

const byte MATRIX_CENTRE_X = MATRIX_CENTER_X - 1;
const byte MATRIX_CENTRE_Y = MATRIX_CENTER_Y - 1;

CRGBArray<NUM_LEDS> leds;
CRGB *matrixleds = (CRGB*)&leds;
//    #ifdef LEDMATRIX
//    // cLEDMatrix defines
//    cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, HORIZONTAL_MATRIX,
//        MATRIX_TILE_H, MATRIX_TILE_V, HORIZONTAL_BLOCKS> ledmatrix(false);
//    #endif
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MATRIX_WIDTH, MATRIX_HEIGHT,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);
#endif





#define mmin(a,b) ((a<b)?(a):(b))
#define mmax(a,b) ((a>b)?(a):(b))

//#include "neomatrix_config.h"
//#include "Drawable.h"






// FastLED::colorutils needs a signature with uint8_t
uint16_t XY_EF( uint8_t x, uint8_t y) {
    return matrix->XY(x,y);
}
// Like XY, but for a mirror image from the top (used by misconfigured code)
int XY2( int x, int y, bool wrap=false) {
    wrap = wrap; // squelch compiler warning
    return matrix->XY(x,MATRIX_HEIGHT-1-y);
}

// but x/y can be bigger than 256
uint16_t XY16( uint16_t x, uint16_t y) {
    return matrix->XY(x,y);
}

int wrapX(int x) {
    if (x < 0 ) return 0;
    if (x >= MATRIX_WIDTH) return (MATRIX_WIDTH-1);
    return x;
}


#endif
