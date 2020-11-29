
#define _SEPARATE_ 1

//#include "../matrix.h"
#include "Effects.h"
Effects effects;

#include "Drawable.h"
#include "Boid.h"
#include "Attractor.h"
#include "Geometry.h"

#include "PatternAttract.h"
PatternAttract attract;
#include "PatternBounce.h"
PatternBounce bounce;
#include "PatternCube.h"
PatternCube cube;
#include "PatternFlock.h"
PatternFlock Aflock;
#include "PatternFlowField.h"
PatternFlowField flowfield;
#include "PatternIncrementalDrift.h"
PatternIncrementalDrift incrementaldrift;
#include "PatternIncrementalDrift2.h"
PatternIncrementalDrift2 incrementaldrift2;
#include "PatternPendulumWave.h"
PatternPendulumWave pendulumwave;
#include "PatternRadar.h"
PatternRadar radar;
#include "PatternSpiral.h"
PatternSpiral spiral;
#include "PatternSpiro.h"
PatternSpiro spiro;
#include "PatternSwirl.h"
PatternSwirl swirl;
#include "PatternWave.h"
PatternWave wave;





AuroraDrawable* items[] = {
    &attract,
    &bounce,
    &cube,
    &Aflock,
    &flowfield,
    &incrementaldrift,
    &incrementaldrift2,
    &pendulumwave,
    &radar,
    &spiral,
    &spiro,
    &swirl,
    &wave,
};
AuroraDrawable *pattern;

int8_t item = -1;
uint8_t numitems = sizeof(items) / sizeof(items[0]);

void loop() {
    int8_t new_pattern = 0;
    char readchar;

    if (Serial.available()) readchar = Serial.read(); else readchar = 0;
    if (readchar) {
  while ((readchar >= '0') && (readchar <= '9')) {
      new_pattern = 10 * new_pattern + (readchar - '0');
      readchar = 0;
      if (Serial.available()) readchar = Serial.read();
  }

  if (new_pattern) {
      Serial.print("Got new pattern via serial ");
      Serial.println(new_pattern);
      item = new_pattern;
  } else {
      Serial.print("Got serial char ");
      Serial.println(readchar);
  }
    }
    if (readchar == 'n')      { Serial.println("Serial => next"); new_pattern = 1;  item++;}
    else if (readchar == 'p') { Serial.println("Serial => previous"); new_pattern = 1; item--;}

    EVERY_N_SECONDS(40) {
  new_pattern = 1;
  item++;
    }

    if (new_pattern || item == -1) { 
  if (item >= numitems) item = 0;
  if (item <= -1) item = numitems - 1;
  pattern = items[item];
  pattern->start();
  Serial.print("Switching to pattern #");
  Serial.print(item);
  Serial.print(": ");
  Serial.println(pattern->name);
  matrix->clear();
    }
    pattern->drawFrame();
    matrix->show();
}

void drawPixelMask(){
//  matrix->clear();
  matrix->drawPixel(0, 0, (uint16_t)CRGB::Red);  
  matrix->drawPixel(MATRIX_WIDTH-1, MATRIX_HEIGHT-1, (uint16_t)CRGB::Yellow);  
  matrix->drawPixel(MATRIX_WIDTH-1, 0, (uint16_t)CRGB::Green);
  matrix->drawPixel(0, MATRIX_HEIGHT-1, (uint16_t)CRGB::Blue);  
  matrix->show();
}

const int MATRIXPIN = 3;

void setup() {
    delay(1000);    
    Serial.begin(115200);

    FastLED.addLeds<WS2812,MATRIXPIN, GRB>(matrixleds, NUMMATRIX).setCorrection(TypicalLEDStrip);
    Serial.print("Neomatrix total LEDs: ");
    Serial.print(NUMMATRIX);
    Serial.print(" running on pin: ");
    Serial.println(MATRIXPIN);
    matrix->begin();

    drawPixelMask();
//    for(;;);

Serial.println("Got following patterns:");
for ( int item = 0; item <=  numitems - 1; item++ ){
  pattern = items[item];
//  pattern->start();  
  Serial.print(item);
  Serial.print(": ");
  Serial.println(pattern->name);
}

//    matrix_setup();
    effects.leds = matrixleds;
    effects.Setup();
    matrix->clear();
}
