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

void convertRG565toCRGB(){
  
}

void dumpPtr(const uint8_t* fdst, uint16_t fsize){
    Serial.println("=====================================");    
    for ( uint16_t i =0; i< fsize; i++ ){
      Serial.printf("%2X ", fdst[i]);            
    }
    Serial.println("=====================================");
}


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

//uint16_t rgb888torgb565(uint8_t *rgb888Pixel)
//{
//    uint8_t red   = rgb888Pixel[0];
//    uint8_t green = rgb888Pixel[1];
//    uint8_t blue  = rgb888Pixel[2];
//
//    uint16_t b = (blue >> 3) & 0x1f;
//    uint16_t g = ((green >> 2) & 0x3f) << 5;
//    uint16_t r = ((red >> 3) & 0x1f) << 11;
//
//    return (uint16_t) (r | g | b);
//}


//uint16_t rgb888toRgb565(CRGB* col){
//  return (
//    ( (uint8_t)( (float) col->r * 31.0f / 255.0f + 0.5f ) && 0x1f ) << 11 +
//    ( (uint8_t)( (float) col->g * 63.0f / 255.0f + 0.5f ) && 0x3f ) << 5 +
//    ( (uint8_t)( (float) col->b * 31.0f / 255.0f + 0.5f ) && 0x1f ) );
//}

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
