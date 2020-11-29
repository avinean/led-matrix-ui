// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#undef _SEPARATE_

#include "soc/rtc_wdt.h"


#include "defs.h"
#include "helpers.h"


#include "WirelessHelper.h"
#include "ServerHandlers.h"
#include "OTAHelper.h"

#include "SPIFFS_functions.h"

#include "gamesHelper.h"
#include "EffectsHelper.h"

//#include "mediaHelper.h"
#include "modesHelper.h"


#include "PatternRadarDraw.h"
PatternRadarDraw radarDraw;



const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

void printLocalTime(tm* timeinfo)
{
//  struct tm timeinfo;
//  if (!getLocalTime(&timeinfo)) {
//    Serial.println("Failed to obtain time");
//  }
  Serial.println(timeinfo, "%A, %B %d %Y %H:%M:%S");
}

int offset = 0;
uint16_t theta = 0;



//=== SEGMENTS ===
#include "Digit.h"
Digit digit0(matrix, 0, 16 - 1 - 9*1, 8, matrix->Color(0, 0, 255));
Digit digit1(matrix, 0, 16 - 1 - 9*2, 8, matrix->Color(0, 0, 255));
Digit digit2(matrix, 0, 16 - 4 - 9*3, 15, matrix->Color(0, 0, 255));
Digit digit3(matrix, 0, 16 - 4 - 9*4, 15, matrix->Color(0, 0, 255));
//Digit digit4(matrix, 0, 63 - 7 - 9*5, 8, matrix->Color(0, 0, 255));
//Digit digit5(matrix, 0, 63 - 7 - 9*6, 8, matrix->Color(0, 0, 255));

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR display_updater() {
    // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&timerMux);
    //isplay.display(70);
//    display.displayTestPattern(70);
    portEXIT_CRITICAL_ISR(&timerMux);
}


void setup() {
  // Time for serial port to work?
  delay(1000);
  randomSeed(analogRead(0));
  led.begin();
  led.on();
  Serial.begin(115200);
  dumpSystemInfo();
  //  pinMode(LED_BUILTIN, OUTPUT);
  //  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    led.blink3();
    while (1) {
      yield(); // Stay here twiddling thumbs waiting
      led.loop();
    }
  }
  //  printSPIFFSInfo();
  listFiles(-1);

  updateFW();

  setupMedia();
  initGfx();
  setupWifiConnection();
  setupServer(&server);
  setupGames();

  setupEffects(matrixleds);

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//  printLocalTime();

  matrix->clear();
  matrix->fillScreen(CRGB::Black);
  matrix->show() ;

  //  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  //  delay(500);
  Serial.println("\r\nInitialisation done.");
  //  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  led.off();

  pattern = items[0];//&swirl;
  pattern->start();

  //items[0]->start();

  __MODE_STOPED_FLAG = false;


//enableGameMode();
//gameDemo = true;
//gameTimer.setInterval(60);
//gameTimer.pause(false);
//gameTimer.reset();

//popTimeout.setInterval(250);
//popTimeout.pause(false);
//popTimeout.reset();
//
//shelfTimer.setInterval(75);
//shelfTimer.pause(false);
//shelfTimer.reset();


//  radarDraw.start();
    gameStart();
  for(;;){
//    radarDraw.drawFrame();
      uint8_t myChar;
      // if there's any serial available, read it:
      while (Serial.available() > 0) {
          // look for the newline. That's the end of your sentence:
          if (Serial.read() == '\n') {
              gameRoutine();
              delay(55);
          }
      }
  }


    gameStart();

    __LAST_MODE = __MODE_NULL;
    __CURRENT_MODE = __MODE_GAME;
    __NEXT_MODE = __MODE_NULL;
    __CURRENT_MODE_FUNC = &__MODES_FNCS_[__CURRENT_MODE];
    __MODE_STOPING_FLAG = false;
    __MODE_STOPED_FLAG = false;



    tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    }
    int hh = timeinfo.tm_hour;
    int mm = timeinfo.tm_min;
    int ss = timeinfo.tm_sec;

    printLocalTime(&timeinfo);

    digit0.Draw(ss % 10);
    digit1.Draw(ss / 10);
//            digit0.Draw(mm % 10);
//            digit1.Draw(mm / 10);
    digit2.Draw(hh % 10);
    digit3.Draw(hh / 10);
}


// bool __MODE_STOPED_FLAG = true;

bool __RENDER_PAUSED = false;

tm prevtimeinfo;
tm* prevtimeinfoPtr = &prevtimeinfo;

void loop() {
    tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    }
    printLocalTime(&timeinfo);
    printLocalTime(&prevtimeinfo);
    Serial.println("\n");
//    unsigned long epoch = ntpClient.GetCurrentTime();
    //Serial.print("GetCurrentTime returned epoch = ");
    //Serial.println(epoch);
//    if (epoch != 0) ntpClient.PrintTime();

//    if (timeinfo != prevtimeinfo) {
        // epoch changes every miliseconds, we only want to draw when digits actually change.
        if (timeinfo.tm_sec!=prevtimeinfo.tm_sec) {
            int s0 = timeinfo.tm_sec % 10;
            int s1 = timeinfo.tm_sec / 10;
            if (s0!=digit0.Value()) digit0.Morph(s0);
            if (s1!=digit1.Value()) digit1.Morph(s1);
            //ntpClient.PrintTime();
//            prevss = ss;
        }

        if (timeinfo.tm_min!=prevtimeinfo.tm_min) {
            int m0 = timeinfo.tm_min % 10;
            int m1 = timeinfo.tm_min / 10;
            if (m0!=digit2.Value()) digit2.Morph(m0);
            if (m1!=digit3.Value()) digit3.Morph(m1);
//            prevmm = mm;
        }

        if (timeinfo.tm_hour!=prevtimeinfo.tm_hour) {
            int h0 = timeinfo.tm_hour % 10;
            int h1 = timeinfo.tm_hour / 10;
//            if (h0!=digit4.Value()) digit4.Morph(h0);
//            if (h1!=digit5.Value()) digit5.Morph(h1);
//            prevhh = hh;
        }
    //        prevEpoch = epoch;
        prevtimeinfo = timeinfo;
//    }
}

void loop321(){

    if ( __NEXT_MODE != __MODE_NULL ){
        if ( !__MODE_STOPING_FLAG ) {
            __MODE_STOPING_FLAG = true;
        }
        if ( __MODE_STOPING_FLAG && __MODE_STOPED_FLAG ){
            __LAST_MODE = __CURRENT_MODE;
            __CURRENT_MODE = __NEXT_MODE;
            __NEXT_MODE = __MODE_NULL;
            __CURRENT_MODE_FUNC = &__MODES_FNCS_[__CURRENT_MODE];
            __MODE_STOPING_FLAG = false;
            __MODE_STOPED_FLAG = false;
        }
    }

    EVERY_N_MILLISECONDS(66) {
        if ( __MODE_STOPING_FLAG && !__MODE_STOPED_FLAG ) {
            __MODE_STOPED_FLAG = true;
        }
        if ( !__MODE_STOPED_FLAG && __CURRENT_MODE_FUNC != NULL ) {
            __CURRENT_MODE_FUNC->func();
        }
        yield();
    }
}

/*
void loop123(){
  EVERY_N_SECONDS(40) {
        new_pattern = 1;
      }
  EVERY_N_SECONDS(1) {
    if ( __CURRENT_MODE != __NEXT_MODE ){
      Serial.printf("__CURRENT_MODE: %d, __NEXT_MODE: %d\n", __CURRENT_MODE,  __NEXT_MODE);
    }
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  
    if ( __NEXT_MODE != __MODE_NULL) {
      Serial.printf("__CURRENT_MODE: %d, __NEXT_MODE: %d\n", __CURRENT_MODE,  __NEXT_MODE);
      if ( !__MODE_STOPED_FLAG ) {
        Serial.printf("Starting mode change");
        __MODE_STOPED_FLAG = true;
      } else {
        __LAST_MODE = __CURRENT_MODE;
        __CURRENT_MODE = __NEXT_MODE;
        __NEXT_MODE = __MODE_NULL;
        __LAST_MODE_FUNC = __CURRENT_MODE_FUNC;
  //      __CURRENT_MODE_FUNC = &__MODES_FNCS_[__NEXT_MODE];
        __MODE_STOPED_FLAG = false;
        if ( !MODE_DRAW_FLAG ) {
          matrix->clear();
          matrix->show();
        }
      }
    }
    
  
      if ( !__MODE_STOPED_FLAG ) {
      yield();
      switch ( __CURRENT_MODE ) {    
        case __MODE_JPG:{
          String path = __JPGS_FOLDER + "/" + __imgName;
  //        doOnJpgFileFoundCallbackWrapper(path.c_str());
          playMediaFile(path.c_str());
          break;
        };
        case __MODE_GIF:{
          String path = __GIFS_FOLDER + "/" + __imgName;
  //        doOnGifFileFoundCallbackWrapper(path.c_str());
          playMediaFile(path.c_str());
          break;
        };
        
        case __MODE_BOOT:{
          display_scrollText_multyline(__BOOT_STR__, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
          FastLED.show();
          break;
        };
        case __MODE_DRAW:{      
          break;
        };
        case __MODE_GAME:{
          gameRoutine();
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
    //      RunAnimationDependingOnPgm();
          FastLED.show();
          break;
        };
        case __MODE_GIF_PLAY:{      
          findFilesInDir(SPIFFS, __GIFS_FOLDER.c_str(), ".gif", 3, doOnMediaFileFoundCallbackWrapper);//doOnGifFileFoundCallbackWrapper);     
          FastLED.show();
          break;
        };
        case __MODE_JPG_PLAY:{      
          findFilesInDir(SPIFFS, __JPGS_FOLDER.c_str(), ".jpg", 3, doOnMediaFileFoundCallbackWrapper);///doOnJpgFileFoundCallbackWrapper);
          FastLED.show();
          break;
        };    
        default:{
          FastLED.show();
          break;
        }
      }
      delay(50);
    } else {
      __RENDER_PAUSED = true;
    }
  }
}
*/

/*
void loop1() {
  Serial.printf("__CURRENT_MODE: %d, __NEXT_MODE: %d\n", __CURRENT_MODE,  __NEXT_MODE);
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)

  if ( __NEXT_MODE != __MODE_NULL) {
    if ( !__MODE_STOPED_FLAG ) {
      Serial.printf("Starting mode change");
      __MODE_STOPED_FLAG = true;
    } else {

      //      pattern = items[0];
      __LAST_MODE_FUNC = __CURRENT_MODE_FUNC;
//      __CURRENT_MODE_FUNC = &__MODES_FNCS_[__NEXT_MODE];
      __MODE_STOPED_FLAG = false;
    }
  }

  if ( !__MODE_STOPED_FLAG ) {
    if ( __NEXT_MODE != __MODE_NULL) {
      __NEXT_MODE = __MODE_NULL;
    }
    if ( !MODE_DRAW_FLAG ) {
      matrix->clear();
      matrix->show();
    }

    EVERY_N_SECONDS(40) {
      new_pattern = 1;
    }
    EVERY_N_MILLISECONDS(20) {
      Serial.printf("__CURRENT_MODE: %d, __NEXT_MODE: %d\n", __CURRENT_MODE,  __NEXT_MODE);
      //      if (!__MODE_STOPED_FLAG){
      //        pattern->drawFrame();
      __CURRENT_MODE_FUNC->func();
      //      pattern->drawFrame();
      matrix->show();
      //      }
    }
  }
    yield();
}
*/

// vim:sts=4:sw=4
