// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#include "defs.h"
#include "helpers.h"

#include "GfxHelper.h"
#include "WirelessHelper.h"
#include "ServerHandlers.h"
#include "OTAHelper.h"

// JPEG decoder library
#include <JPEGDecoder.h>

#include "JPEG_functions.h"
#include "SPIFFS_functions.h"

#include "GifPlayer.h"
GifPlayer gifPlayer;



TaskHandle_t Task1;

// LED pins
const int LED_BUILTIN = 2;


void playGif(const char * dir, const char * fileName){
//void playGif(File* file){
//  File root = SPIFFS.open(dir);
  File file = SPIFFS.open(String(dir)+String("/") + String(fileName), "r");
  if ( file && String(file.name()).endsWith(".gif") ){
    Serial.print("  FILE: ");
    Serial.print(file.name());
    Serial.print("\tSIZE: ");
    Serial.println(file.size());
  
//    String fileName = file->name();
    Serial.println("!!GIF!!");  
    Serial.print("Reading ");
//    Serial.println(fileName);
  
    File imageFile = SPIFFS.open(fileName, "r");
    if (!imageFile) {
        Serial.println("Failed to open");
        return;
    }
    
    gifPlayer.setFile(imageFile);
  
    for (uint8_t c=0; c<10; c++) {
      if (!gifPlayer.parseGifHeader()) {
        imageFile.close();
        Serial.println("No gif header");
        return;
      }

      matrix->clear();
      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
      Serial.println("Processing gif");
      int result;
      do {
//                  gifPlayer.drawFrame();
        result = gifPlayer.drawFrame();
        matrix->show();
        delay(50);
      } while (result != ERROR_FINISHED);
      imageFile.seek(0);
    }

    Serial.println("Gif finished");
    imageFile.close();
    delay(1000);
  }

}

uint8_t gifIdx = 0;
const uint8_t gifsCnt = 4;
const String gifsPath[gifsCnt] = { "/gifs/32anim_balls.gif", "/gifs/32anim_flower.gif", "/gifs/32anim_photon.gif" };

void loop() {
//  Serial.print("loop() running on core ");
//  Serial.println(xPortGetCoreID());

//  gifIdx = ( ( gifIdx + 1 ) >= gifsCnt ? 0 : gifIdx + 1 );
//  playGif(gifsPath[gifIdx]);
//  doLoop();
}


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  for(;;){
//    Serial.print("Task1 running on core ");
//    Serial.println(xPortGetCoreID());
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  } 
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

//  display_resolution();
//  delay(2000);
//
matrix->clear();
//  matrix->fillScreen(LED_BLUE_LOW);
  matrix->show();

findFilesInDir(SPIFFS, "/", ".gif", 3, doOnFileFoundCallbackWrapper);

//findFilesInDir(SPIFFS, "/", ".gif", 3, doOnFileFoundCallbackWrapper);
//if ( !gifs.IsEmpty() ){
//  for ( int i = 0; i < gifs.Count(); i++ ){
//    playGif(gifs[i]);  
//  }
//}
for(;;);


  
//  listFiles(); // Lists the files so you can see what is in the SPIFFS
//  listFiles(); // Lists the files so you can see what is in the SPIFFS
//  drawJpeg("/_1.jpg", 0 , 0);     // 240 x 320 image
//  //drawJpeg("/Baboon40.jpg", 0, 0); // 320 x 480 image
//  delay(2000);
//  createArray("/_1.jpg");
//  for(;;);

//  playGif("/gifs/32anim_balls.gif");

  setupWifiConnection();
  setupServer(&server);
  
//  serverInit();
  printSPIFFSInfo();
  spiffsListDir();
//  listFiles(); // Lists the files so you can see what is in the SPIFFS

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  
  delay(500);
  Serial.println("\r\nInitialisation done.");
}

// vim:sts=4:sw=4
