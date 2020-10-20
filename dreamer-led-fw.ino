#include "settings.h"
#include "globals.h"

#include "ServerHelper.h"
#include "MatrixHelper.h"

//#include "utility_funx.h"
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
  setupServer();  
}


void loop() {
  switch ( __CURRENT_MODE ) {
    case __MODE_GAME:{
      snakeRoutine();
//      delay(50);
      break;
    };
    case __MODE_CANVAS:{
      fillCanvas(bitmap);
      FastLED.show();
//      delay(500);
      break;
    };
    case __MODE_TEXT:{      
      fillString(_runningString, __RUNNING_STRING_COLOR);
      FastLED.show();
//      delay(50);
      break;
    };
    default:{
      FastLED.show();
      break;
    }
  }
//  EVERY_N_SECONDS(1) { }  
  delay(200);
}
