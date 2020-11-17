#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "defs.h"
#include "globals.h"
#include "SPIFFS.h"

#include "GifPlayer.h"

#include "Morse.h"

// string: string to parse
// c: delimiter
// returns number of items parsed
String strings_for_split[10];

int getCharCountInString(String string, char del){  
}

int split(String string, char del){
  Serial.printf("split(\"%s\", '%c'):\n", string, del);
  String data = "";
  int bufferIndex = 0;

  for (int i = 0; i < string.length(); ++i){
    char c = string[i];
    
    if (c != del){
      data += c;
    } else {
      data += '\0';
      Serial.printf("buf[%d]: %s\n", bufferIndex, data);
      strings_for_split[bufferIndex++] = data;
      data = "";
    }
  }
  if (string.length() > string.lastIndexOf(del)){
    data += '\0';
    Serial.printf("buf[%d]: %s\n", bufferIndex, data);
    strings_for_split[bufferIndex++] = data;    
  }
  return bufferIndex;
}

Morse morse(LED_BUILTIN, 2000 / portTICK_PERIOD_MS, 204 ); // 204 = 0b11001100

/* this function will be invoked when additionalTask was created */
void blinkingTask( void * parameter ){
/* loop forever */
  for(;;){
//    Serial.println("this is another Task");
//    delay(1000);
    morse.patternNext();
    vTaskDelay( morse.getDelay() / portTICK_PERIOD_MS );
  }
}

// Translate the x/y coordinates into the right index in the
// framebuffer.
// The Smartmatrix has a simple line by line layout, no 
// serpentines. It safed 2 fps to keep this function short.
// The function is called (sometimes) over 200 000 times per second!

uint16_t XY( uint8_t x, uint8_t y) {
  uint16_t i;
  i = (y * MX_WIDTH) + x;
  return i;
}


uint32_t getPixColorXY ( uint16_t x, uint16_t y ){
  return (uint32_t)leds[ XY(x, y)];
}

void dumpPtr(const uint8_t* fdst, uint16_t fsize){
    Serial.println("=====================================");    
    for ( uint16_t i =0; i< fsize; i++ ){
      Serial.printf("%2X ", fdst[i]);            
    }
    Serial.println("=====================================");
}


//void playGif(const char * dir, const char * fileName){
void playGif(File* file){
//  File root = SPIFFS.open(dir);
//  File file = SPIFFS.open(String(dir)+String("/") + String(fileName), "r");
  if ( (*file) && String(file->name()).endsWith(".gif") ){
    Serial.println("!!GIF!!");  
    Serial.print("  FILE: ");
    Serial.print(file->name());
    Serial.print("\tSIZE: ");
    Serial.println(file->size());
  
//    String fileName = file->name();    
    Serial.print("Reading ");
//    Serial.println(fileName);
  
//    File imageFile = SPIFFS.open(fileName, "r");
//    if (!imageFile) {
//        Serial.println("Failed to open");
//        return;
//    }
    
    gifPlayer.setFile(*file);
  
    for (uint8_t c=0; c<10; c++) {
      if (!gifPlayer.parseGifHeader()) {
        file->close();
        Serial.println("No gif header");
        return;
      }

      matrix->clear();
      gifPlayer.parseLogicalScreenDescriptor();
      gifPlayer.parseGlobalColorTable();
      Serial.println("Processing gif");
      int result;
      do {
        result = gifPlayer.drawFrame();
        matrix->show();
        delay(10);
      } while (result != ERROR_FINISHED);
      file->seek(0);
    }

    Serial.println("Gif finished");
    file->close();
    delay(100);
  }

}

void dumpSystemInfo(){
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
   
  Serial.println("Hardware info:");

  printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  printf("silicon revision %d, ", chip_info.revision);

  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
          (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

  //get chip id
  String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
   
  Serial.printf("Chip id: %s\n", chipId.c_str());
}


#endif
