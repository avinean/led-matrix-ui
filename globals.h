#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defs.h"

volatile TaskHandle_t currentGfxTask;
volatile TaskHandle_t Task1;
volatile TaskHandle_t startupTask;

volatile uint8_t _EFFECT_ID = 0;
volatile uint8_t _EFFECT_SPEED = 50;


bitmapInfo drawTaskBitmapInfo;
//uint8_t drawTaskBitmapBuffer[BITMAP_SIZE*4];
volatile uint16_t drawTaskBitmapBuffer[NUM_LEDS];


const String CONFIG_IDF_TARGET = "esp32";

#include <ESPAsyncWebServer.h>
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



CRGB leds[NUM_LEDS];
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, MX_WIDTH, MX_HEIGHT,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

volatile uint8_t _MX_ROTATION = 0;
    
// тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка
#define __MODE_CANVAS 0
#define __MODE_TEXT   1
#define __MODE_GAME   2
volatile unsigned char __CURRENT_MODE = __MODE_CANVAS;//__MODE_TEXT;

//volatile 
String _GAME_NAME_;
volatile byte _GAME_BUTTONS_ = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

//volatile 
String _RUN_TEXT_ = "hello world";
volatile int _RUN_TEXT_SPEED_ = 200;
CRGB __RUNNING_STRING_COLOR = CRGB::Yellow;
CRGB __RUNNING_STRING_BACKGROUND_COLOR = CRGB::Blue;

#endif
