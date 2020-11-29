#ifndef _EFFECTSHELPER_H_
#define _EFFECTSHELPER_H_

#define DEBUG 0

#include "globals.h"
#include "helpers.h"

#include "effects/PatternAttract.h"
PatternAttract attract;
#include "effects/PatternBounce.h"
PatternBounce bounce;
#include "effects/PatternCube.h"
PatternCube cube;
#include "effects/PatternFlock.h"
PatternFlock Aflock;
#include "effects/PatternFlowField.h"
PatternFlowField flowfield;
#include "effects/PatternIncrementalDrift.h"
PatternIncrementalDrift incrementaldrift;
#include "effects/PatternIncrementalDrift2.h"
PatternIncrementalDrift2 incrementaldrift2;
#include "effects/PatternPendulumWave.h"
PatternPendulumWave pendulumwave;
#include "effects/PatternRadar.h"
PatternRadar radar;
#include "effects/PatternSpiral.h"
PatternSpiral spiral;
#include "effects/PatternSpiro.h"
PatternSpiro spiro;
#include "effects/PatternSwirl.h"
PatternSwirl swirl;
#include "effects/PatternWave.h"
PatternWave wave;

//#include "PatternRadarDraw.h"
//AuroraDrawable* radarDraw();
//#include "RunningTextDraw.h"
//AuroraDrawable* textDraw();





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


uint8_t numitems = sizeof(items) / sizeof(items[0]);
int8_t new_pattern = 0;


void dumpEffects(){
  Serial.println("Got following patterns:");
  for ( int item = 0; item <=  numitems - 1; item++ ){
    pattern = items[item];
    Serial.print(item);
    Serial.print(": ");
    Serial.println(pattern->name);
  }
}


void RunAnimationDependingOnPgm() {
  for ( int i = 0; i < 333; i++ ){
    if ( ! __MODE_STOPED_FLAG ) {
      if (new_pattern || _EFFECT_ID == -1) { 
        if (_EFFECT_ID >= numitems) 
          _EFFECT_ID = random(0, numitems-1);
        new_pattern = 0;
        pattern = items[_EFFECT_ID];
        pattern->start();
        Serial.print("Switching to pattern #");
        Serial.print(_EFFECT_ID);
        Serial.print(": ");
        Serial.println(pattern->name);
        matrix->clear();
      }
      pattern->drawFrame();
      matrix->show();    
      delay(__SPEED__);
    } else
      break;
  }
}

void setupEffects(CRGB *leds){
  effects.leds = leds;
  effects.Setup();

#if DEBUG == 1
  dumpEffects();
#endif
}

#endif
