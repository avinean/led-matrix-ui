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


/*
for(;;);
  

int _SZ = 9;

uint16_t cr = matrix->Color(0xff, 0x00, 0x00);
uint16_t cg = matrix->Color(0x00, 0xff, 0x00);
uint16_t cb = matrix->Color(0x00, 0x00, 0xff);
uint16_t _tbf[_SZ] = {
  cr, 0x00, 0xFF,
  0x00, cg, 0x00,
  0xFF, 0x00, cb
};
CRGB _crgbtbf[_SZ] = {
  CRGB::Red,  0x00,         CRGB::Yellow,
  0x00,       CRGB::Green,  0x00,
  CRGB::Cyan, 0x00,         CRGB::Blue
};
uint16_t _cvtbf[_SZ];

for ( uint8_t _idx = 0; _idx < _SZ; _idx++ ){
  _cvtbf[_idx] = matrix->Color(_crgbtbf[_idx].r, _crgbtbf[_idx].g, _crgbtbf[_idx].b);
}

matrix->clear();
matrix->drawRGBBitmap(3, 3, (const unsigned short*)&_cvtbf, 3, 3);
matrix->show();
Serial.printf("dumpPtr(leds)\n");
dumpPtr((const uint8_t*)&leds, BITMAP_SIZE );
//for(;;);
*/

//Serial.printf("test text buf\n");
//writeToFile("/test.bin",(const uint8_t*)"QWERTY", 6);
//dumpFile("/test.bin");
//
//Serial.printf("dumpPtr(RGB_bmp)\n");
//dumpPtr((const uint8_t*)RGB_bmp, 64);
//writeToFile("/test1.bin",(const uint8_t*)RGB_bmp, 64);
//
//uint8_t _buf[512];
//readFromFile("/test1.bin",(const uint8_t*)&_buf, 64 );
//Serial.printf("dumpPtr(_buf)\n");
//dumpPtr((const uint8_t*)&_buf, 64);
//
//bitmapInfo bmi = (bitmapInfo){ (const short unsigned int*)&_buf, 8, 8, 0 };
////bitmapInfo bmi = (bitmapInfo){ (const short unsigned int*)&RGB_bmp, 8, 8, 0 };
////display_panOrBounceBitmap(&bmi);
//matrix->clear();
//Serial.printf("fixdrawRGBBitmap(RGB_bmp[0])\n");
//fixdrawRGBBitmap(0, 0, RGB_bmp[0], 8, 8);
//matrix->show();
//delay(5000);
//
//matrix->clear();
//Serial.printf("memmove( &leds, _buf )\n");
//memmove( &leds, _buf, BITMAP_SIZE );
//matrix->show();
//dumpPtr((const uint8_t*)&leds, 64);
//
//for(;;){
//  Serial.printf("display_panOrBounceBitmap(&bmi)\n");
//  display_panOrBounceBitmap(&bmi);
//};



//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    startupTaskCode,   /* Task function. */
                    "startup",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    (void**)&startupTask,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  


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
   matrix->clear();
   matrix->fillScreen(CRGB::Black);
   matrix->show();
 }
  delay(500);
  Serial.println("\r\nInitialisation done.");
}

// vim:sts=4:sw=4
