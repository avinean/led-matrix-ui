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

uint16_t rgb888toRgb565(CRGB col){
  return (
    ( (uint8_t)( (float) col.r * 31.0f / 255.0f + 0.5f ) && 0x1f ) << 11 +
    ( (uint8_t)( (float) col.g * 63.0f / 255.0f + 0.5f ) && 0x3f ) << 5 +
    ( (uint8_t)( (float) col.b * 31.0f / 255.0f + 0.5f ) && 0x1f ) );
}

void dumpSystemInfo(){
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
   
  Serial.println("Hardware info:");

  printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET.c_str(),
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
