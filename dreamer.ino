// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#include "defs.h"
#include "helpers.h"

#include "GfxHelper.h"
#include "WirelessHelper.h"
#include "ServerHandlers.h"
#include "OTAHelper.h"
#include "EffectsHelper.h"

// JPEG decoder library
#include <JPEGDecoder.h>

#include "JPEG_functions.h"
#include "SPIFFS_functions.h"

#include "g_snake.h"




void doOnGifFileFoundCallbackWrapper(const char * file){
  Serial.print("  FILE: ");
  Serial.println(file);
  String d = String(file); //String(dir) + (String(dir).endsWith("/") ? "" : "/") + String(file);  
  Serial.printf("Str file: %s", d);
  File f = SPIFFS.open(d, "r");
  playGif(&f);
}

void doOnJpgFileFoundCallbackWrapper(const char * file){
//  Serial.print("  FILE: ");
//  Serial.println(file);
//  String d = String(file); //String(dir) + (String(dir).endsWith("/") ? "" : "/") + String(file);  
//  Serial.printf("Str file: %s", d);
//  File f = SPIFFS.open(d, "r");
//  playGif(&f);
  matrix->clear();
  drawJpeg(file, 0 , 0);     // 240 x 320 image
  delay(__SPEED__);
  matrix->show();
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  switch ( __CURRENT_MODE ) {    
    case __MODE_BOOT:{
      display_scrollText_multyline(__BOOT_STR__, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
      FastLED.show();
      break;
    };
    case __MODE_DRAW:{      
      break;
    };
    case __MODE_GAME:{
      snakeRoutine();
      break;
    };
    case __MODE_CANVAS:{
      display_panOrBounceBitmap(&drawTaskBitmapInfo);
      FastLED.show();
      break;
    };
    case __MODE_TEXT:{      
      display_scrollText(_RUN_TEXT_, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
      FastLED.show();
      break;
    };
    case __MODE_EFFECTS:{      
      RunAnimationDependingOnPgm();
      FastLED.show();
      break;
    };
    case __MODE_GIF_PLAY:{      
      findFilesInDir(SPIFFS, __GIFS_FOLDER.c_str(), ".gif", 3, doOnGifFileFoundCallbackWrapper);     
      FastLED.show();
      break;
    };
    case __MODE_JPG_PLAY:{      
      findFilesInDir(SPIFFS, __JPGS_FOLDER.c_str(), ".jpg", 3, doOnJpgFileFoundCallbackWrapper);
      FastLED.show();
      break;
      /*
      File root = SPIFFS.open(__JPGS_FOLDER);
      while (File file = root.openNextFile()) {
        String strname = file.name();
        // If it is not a directory and filename ends in .jpg then load it
        if (!file.isDirectory() && strname.endsWith(".jpg")) {
          Serial.printf("File: %s\n", strname ); 
          matrix->clear();
          drawJpeg(strname.c_str(), 0 , 0);     // 240 x 320 image
          matrix->show();
#if DEBUG == 1          
          Serial.printf("dumpPtr(leds)\n");
          dumpPtr((const uint8_t*)&leds, NUM_LEDS );
#endif          
          delay(1000);
        }
      }
//      FastLED.show();
      break;*/
    };    
    default:{
      FastLED.show();
      break;
    }
  }
//  EVERY_N_SECONDS(1) { }  
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW    
//  morse.patternNext();
//  delay(10);
}


void setup() {
  // Time for serial port to work?
  delay(1000);
  Serial.begin(115200);
  dumpSystemInfo();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  
  updateFW();
  
  initGfx();  
  setupWifiConnection();
  setupServer(&server);
  
  matrix->clear();
  matrix->fillScreen(CRGB::Black);
  matrix->show();
  setupEffects();
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);
  Serial.println("\r\nInitialisation done.");
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW

  __CURRENT_MODE = __MODE_BOOT;
  __MODE_STOP__ = false;
}

// vim:sts=4:sw=4
