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


String sUUID(uint8_t len){
  String res[len];
  for ( uint8_t i = 0; i < len; i++ ){    
    res[i] = ( i % 2 == 0 ? (char)random(48, 57) : (char)random(97, 122) );
  }
}

funcPtrInfo funcPtrInfoBld(String name, int flag, funcPtr ptr){
  funcPtrInfo res;
  res.name = name;
  res.flags = flag;
  res.func = ptr;
  return  res;
}


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

uint16_t XY( uint8_t x, uint8_t y, uint8_t w) {
  uint16_t i;
  i = (y * w) + x;
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




void doOnGifFileFoundCallbackWrapper(const char * file){
  Serial.print("  FILE: ");
  Serial.println(file);
  String d = String(file); //String(dir) + (String(dir).endsWith("/") ? "" : "/") + String(file);  
  Serial.printf("Str file: %s", d);
  File f = SPIFFS.open(d, "r");
  playGif(&f);
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
