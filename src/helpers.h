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



void startGameTimer(TimerCallbackFunction_t func, uint8_t interval, uint8_t id) {
    tmr = xTimerCreate("MyTimer", pdMS_TO_TICKS(interval), pdTRUE, ( void * )id, func);
    if( xTimerStart(tmr, 10 ) != pdPASS ) {
        printf("Timer start error");
    }

}


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
  Serial.printf("split(\"%s\", '%c'):\n", string.c_str(), del);
  String data = "";  int bufferIndex = 0;

  for (int i = 0; i < string.length(); ++i){
    char c = string[i];
    
    if (c != del){
      data += c;
    } else {
      data += '\0';
      #if DEBUG == 1
      Serial.printf("buf[%d]: %s\n", bufferIndex, data);
      #endif 
      strings_for_split[bufferIndex++] = data;
      data = "";
    }
  }
  if (string.length() > string.lastIndexOf(del)){
    data += '\0';
    #if DEBUG == 1
    Serial.printf("buf[%d]: %s\n", bufferIndex, data);
    #endif
    strings_for_split[bufferIndex++] = data;    
  }
  return bufferIndex;
}




void dumpSystemInfo(){
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    Serial.println("Hardware info:");

    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           String((chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "").c_str(),
           String((chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "").c_str());

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           String((chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external").c_str());

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    //get chip id
    String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
    chipId.toUpperCase();

    Serial.printf("Chip id: %s\n", chipId.c_str());
}


#endif
