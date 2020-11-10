// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#define INCLUDE_vTaskDelete 1

#include "defs.h"
#include "helpers.h"

#include "GfxHelper.h"
#include "WirelessHelper.h"
#include "ServerHandlers.h"
#include "OTAHelper.h"
#include "TasksHelper.h"
#include "EffectsHelper.h"

// JPEG decoder library
#include <JPEGDecoder.h>

#include "JPEG_functions.h"
#include "SPIFFS_functions.h"




// LED pins
const int LED_BUILTIN = 2;



SemaphoreHandle_t syncSemaphore;

uint8_t gifIdx = 0;
const uint8_t gifsCnt = 4;
const String gifsPath[gifsCnt] = { "/gifs/32anim_balls.gif", "/gifs/32anim_flower.gif", "/gifs/32anim_photon.gif" };

void loop() {
//  vTaskDelete( NULL );
  yield();
//  effectsLoop();
  vTaskDelay( pdMS_TO_TICKS( 50 ) );


//  Serial.print("loop() running on core ");
//  Serial.println(xPortGetCoreID());

//  gifIdx = ( ( gifIdx + 1 ) >= gifsCnt ? 0 : gifIdx + 1 );
//  playGif(gifsPath[gifIdx]);
//  doLoop();
//display_scrollText();
//display_scrollText("T E S T", CRGB::Yellow, CRGB::Cyan);
}



//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  for(;;){
//    Serial.print("Task1 running on core ");
//    Serial.println(xPortGetCoreID());
    digitalWrite(LED_BUILTIN, HIGH);    
//    delay(1000);
    vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    digitalWrite(LED_BUILTIN, LOW);
//    delay(1000);
    vTaskDelay( pdMS_TO_TICKS( 1000 ) );
  } 
}

//Task1code: blinks an LED every 1000 ms
void startupTaskCode( void * pvParameters ){
  findFilesInDir(SPIFFS, "/", ".gif", 3, doOnFileFoundCallbackWrapper);     
}



void doOnFileFoundCallbackWrapper(const char * dir, const char * file){
  playGif(dir, file);
}

void setup() {
  // Time for serial port to work?
  delay(1000);
  Serial.begin(115200);
  dumpSystemInfo();
  pinMode(LED_BUILTIN, OUTPUT);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }

  
  updateFW();
  initGfx();

/* !!!!
File root = SPIFFS.open("/jpg");
while (File file = root.openNextFile()) {
  String strname = file.name();
  // If it is not a directory and filename ends in .jpg then load it
  if (!file.isDirectory() && strname.endsWith(".jpg")) {
    Serial.printf("File: %s\n", strname ); 
//    loadFile(strname.c_str());
    matrix->clear();
//    createArray("/p3x3.jpg");
    drawJpeg(strname.c_str(), 0 , 0);     // 240 x 320 image
    matrix->show();
    Serial.printf("dumpPtr(leds)\n");
    dumpPtr((const uint8_t*)&leds, NUM_LEDS );
    delay(1000);
  }
}
!!!! */


//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
//  xTaskCreatePinnedToCore(
//                    startupTaskCode,   /* Task function. */
//                    "startup",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    2,           /* priority of the task */
//                    (void**)&startupTask,      /* Task handle to keep track of created task */
//                    1);          /* pin task to core 0 */                  


  setupWifiConnection();
  setupServer(&server);
  
  printSPIFFSInfo();
  spiffsListDir();

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    (void**)&Task1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  


  
  // Use the handle to delete the task.
 if( startupTask != NULL ){
   vTaskDelete( startupTask );   
 }
 matrix->clear();
 matrix->fillScreen(CRGB::Black);
 matrix->show();
 setupEffects();
  delay(500);
  Serial.println("\r\nInitialisation done.");
}

// vim:sts=4:sw=4
