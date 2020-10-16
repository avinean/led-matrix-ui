


#include "settings.h"
#include "globals.h"

#include "ServerHelper.h"
#include "MatrixHelper.h"

#include "utility_funx.h"
#include "runningText.h"

#include "g_snake.h"

//cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

void drawPixelMask(){
  leds[ XY(0, 0)]  = CRGB::Red;  
  leds[ XY(MX_WIDTH-1, MX_HEIGHT-1)]  = CRGB::Yellow;    
  leds[ XY(MX_WIDTH-1, 0)]  = CRGB::Green;  
  leds[ XY(0, MX_HEIGHT-1)]  = CRGB::Blue;
}

unsigned char _resp1[BITMAP_SIZE];  
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  setupMatrix();  
  drawPixelMask();
//  dumpPixel(0,0);
//  dumpPixel(MX_WIDTH-1, MX_HEIGHT-1);
//  dumpPixel(MX_WIDTH-1, 0);
//  dumpPixel(0, MX_HEIGHT-1);  
  getCanvasPixels(_resp1);
  dumpCanvasPixels(_resp1);

//  delay(5000);
  
  setupServer();  
}


//uint8_t hue;
//int16_t counter;
//void applyEffects(){
//    int16_t sx, sy, x, y;
//  uint8_t h;
//
//  FastLED.clear();
//  
//  h = hue;
//  if (counter < 1125)
//  {
//    // ** Fill LED's with diagonal stripes
//    for (x=0; x<(leds->Width()+leds->Height()); ++x)
//    {
//      leds->DrawLine(x - leds->Height(), leds->Height() - 1, x, 0, CHSV(h, 255, 255));
//      h+=16;
//    }
//  }
//  else
//  {
//    // ** Fill LED's with horizontal stripes
//    for (y=0; y<leds->Height(); ++y)
//    {
//      leds->DrawLine(0, y, leds->Width() - 1, y, CHSV(h, 255, 255));
//      h+=16;
//    }
//  }
//  hue+=4;
//
//  if (counter < 125)
//    ;
//  else if (counter < 375)
//    leds->HorizontalMirror();
//  else if (counter < 625)
//    leds->VerticalMirror();
//  else if (counter < 875)
//    leds->QuadrantMirror();
//  else if (counter < 1125)
//    leds->QuadrantRotateMirror();
//  else if (counter < 1250)
//    ;
//  else if (counter < 1500)
//    leds->TriangleTopMirror();
//  else if (counter < 1750)
//    leds->TriangleBottomMirror();
//  else if (counter < 2000)
//    leds->QuadrantTopTriangleMirror();
//  else if (counter < 2250)
//    leds->QuadrantBottomTriangleMirror();
//
//  counter++;
//  if (counter >= 2250)
//    counter = 0;
//}

void loop() {
//  fillCanvas(bitmap);
//  if ( _runningStringActive && ( _runningString.length() > 0 ) ) {
  if ( _runningStringActive == true ) {
    fillString(_runningString, CRGB::Red);
  }
//  EVERY_N_SECONDS(1) {
//    fillCanvas(bitmap); 
//  } 

snakeRoutine();
    drawPixelMask();
    
//  }
  
  

  FastLED.show();
  delay(30);
}
