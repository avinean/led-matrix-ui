#ifndef _MATRIXHELPER_H_
#define _MATRIXHELPER_H_

#include <FastLED.h>
#include <FastLED_GFX.h>



CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);
//GFXcanvas canvas(MX_WIDTH, MX_HEIGHT);



void setupMatrix(){
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  
}


// Helper functions for an two-dimensional XY matrix of pixels.
// Simple 2-D demo code is included as well.
//
//     XY(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             No error checking is performed on the ranges of x and y.
//
//     XYsafe(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             Error checking IS performed on the ranges of x and y, and an
//             index of "-1" is returned.  Special instructions below
//             explain how to use this without having to do your own error
//             checking every time you use this function.  
//             This is a slightly more advanced technique, and 
//             it REQUIRES SPECIAL ADDITIONAL setup, described below.





// Set 'kMatrixSerpentineLayout' to false if your pixels are 
// laid out all running the same way, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19
//
// Set 'kMatrixSerpentineLayout' to true if your pixels are 
// laid out back-and-forth, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15
//
// Bonus vocabulary word: anything that goes one way 
// in one row, and then backwards in the next row, and so on
// is call "boustrophedon", meaning "as the ox plows."


// This function will return the right 'led index number' for 
// a given set of X and Y coordinates on your matrix.  
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.  
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < MX_WIDTH; x++) {
//      for( uint8_t y = 0; y < MX_HEIGHT; y++) {
//      
//        // Here's the x, y to 'led index' in action: 
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//      
//      }
//    }
//
//
uint16_t XY( uint8_t y, uint8_t x)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * MX_WIDTH) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (MX_WIDTH - 1) - x;
      i = (y * MX_WIDTH) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * MX_WIDTH) + x;
    }
  }
  
  return i;
}


// Once you've gotten the basics working (AND NOT UNTIL THEN!)
// here's a helpful technique that can be tricky to set up, but 
// then helps you avoid the needs for sprinkling array-bound-checking
// throughout your code.
//
// It requires a careful attention to get it set up correctly, but
// can potentially make your code smaller and faster.
//
// Suppose you have an 8 x 5 matrix of 40 LEDs.  Normally, you'd
// delcare your leds array like this:
//    CRGB leds[40];
// But instead of that, declare an LED buffer with one extra pixel in
// it, "leds_plus_safety_pixel".  Then declare "leds" as a pointer to
// that array, but starting with the 2nd element (id=1) of that array: 
//    CRGB leds_with_safety_pixel[41];
//    CRGB* const leds( leds_plus_safety_pixel + 1);
// Then you use the "leds" array as you normally would.
// Now "leds[0..N]" are aliases for "leds_plus_safety_pixel[1..(N+1)]",
// AND leds[-1] is now a legitimate and safe alias for leds_plus_safety_pixel[0].
// leds_plus_safety_pixel[0] aka leds[-1] is now your "safety pixel".
//
// Now instead of using the XY function above, use the one below, "XYsafe".
//
// If the X and Y values are 'in bounds', this function will return an index
// into the visible led array, same as "XY" does.
// HOWEVER -- and this is the trick -- if the X or Y values
// are out of bounds, this function will return an index of -1.
// And since leds[-1] is actually just an alias for leds_plus_safety_pixel[0],
// it's a totally safe and legal place to access.  And since the 'safety pixel'
// falls 'outside' the visible part of the LED array, anything you write 
// there is hidden from view automatically.
// Thus, this line of code is totally safe, regardless of the actual size of
// your matrix:
//    leds[ XYsafe( random8(), random8() ) ] = CHSV( random8(), 255, 255);
//
// The only catch here is that while this makes it safe to read from and
// write to 'any pixel', there's really only ONE 'safety pixel'.  No matter
// what out-of-bounds coordinates you write to, you'll really be writing to
// that one safety pixel.  And if you try to READ from the safety pixel,
// you'll read whatever was written there last, reglardless of what coordinates
// were supplied.



uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= MX_WIDTH) return -1;
  if( y >= MX_HEIGHT) return -1;
  return XY(x,y);
}

void fillCanvas(unsigned char bmap[]){
// TODO: m.b. use memmove instead?
//  Copy ten led colors from leds[src .. src+9] to leds[dest .. dest+9]
//  memmove( &leds[dest], &leds[src], 10 * sizeof( CRGB) );
  int idx = 0;
  for ( int y = 0; y < MX_HEIGHT; y++ ){
    for ( int x = 0; x < MX_WIDTH; x++ ){
      idx = (x + y * MX_WIDTH) * DEPTH;
//      if ( 720 < bitmap[idx] + bitmap[idx+1]  + bitmap[idx+2] ){
//        leds[ XY(x, y)] = CRGB::Black;
//      } else {
        leds[ XY(x, y)].r = bmap[idx ];
        leds[ XY(x, y)].g = bmap [idx+1 ];
        leds[ XY(x, y)].b = bmap [ idx+2 ];
//      }
    }
  }
}

void getCanvasPixels(unsigned char bmap[]){
// TODO: m.b. use memmove instead?
//  Copy ten led colors from leds[src .. src+9] to leds[dest .. dest+9]
//  memmove( &leds[dest], &leds[src], 10 * sizeof( CRGB) );
  int idx = 0;
  for ( int y = 0; y < MX_HEIGHT; y++ ){
    for ( int x = 0; x < MX_WIDTH; x++ ){
      idx = (x + y * MX_WIDTH) * DEPTH;
//      if ( 720 < bitmap[idx] + bitmap[idx+1]  + bitmap[idx+2] ){
//        leds[ XY(x, y)] = CRGB::Black;
//      } else {
        bmap[idx ] = leds[ XY(x, y)].r;
        bmap [idx+1 ] = leds[ XY(x, y)].g;
        bmap [ idx+2 ] = leds[ XY(x, y)].b;
//      }
    }
  }
}

// получить номер пикселя в ленте по координатам
uint16_t getPixelNumber(int8_t x, int8_t y) {
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {               // если чётная строка
    return (THIS_Y * _WIDTH + THIS_X);
  } else {                                              // если нечётная строка
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}

// функция отрисовки точки по координатам X Y
void drawPixelXY(int8_t x, int8_t y, CRGB color) {
  if (x < 0 || x > MX_WIDTH - 1 || y < 0 || y > MX_HEIGHT - 1) return;
  int thisPixel = getPixelNumber(x, y) * SEGMENTS;
  for (byte i = 0; i < SEGMENTS; i++) {
    leds[thisPixel + i] = color;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisSegm) {
  int thisPixel = thisSegm * SEGMENTS;
  if (thisPixel < 0 || thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(getPixelNumber(x, y));
}

void dumpCanvasPixels(unsigned char bmap[]){
// TODO: m.b. use memmove instead?
//  Copy ten led colors from leds[src .. src+9] to leds[dest .. dest+9]
//  memmove( &leds[dest], &leds[src], 10 * sizeof( CRGB) );
  int idx = 0;
  Serial.printf("\n\n" );
  for ( int x = 0; x < MX_WIDTH; x++ )
    Serial.printf("    %6d|", x );
  Serial.printf("\n");
  for ( int y = 0; y < MX_HEIGHT; y++ ){
    for ( int x = 0; x < MX_WIDTH; x++ ){
      if ( x == 0 )
        Serial.printf("%4d", y);
      idx = (x + y * MX_WIDTH) * DEPTH;
      Serial.printf("%2X%2X%2X|", bmap[idx], bmap[idx+1], bmap[idx+2] );
    }
    Serial.printf("\n");    
  }
}

CRGB getCanvasPixel(unsigned char x, unsigned char y){
  return leds[ XY(x, y)];
}

void dumpPixel(unsigned char x, unsigned char y){
  CRGB p = getCanvasPixel(x,y);
  Serial.printf("%2dx%2d:%2X%2X%2X\n", x, y, p.r, p.g, p.b );
}

#endif //_MATRIXHELPER_H_
