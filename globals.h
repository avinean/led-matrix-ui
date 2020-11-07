#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defs.h"

#include "ESPAsyncWebServer.h"
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 4
#define MX_COLORS 3
#define MX_WIDTH 16
#define MX_HEIGHT 16
#define NUM_LEDS (MX_WIDTH*MX_HEIGHT)
#define BITMAP_SIZE ( NUM_LEDS * MX_COLORS )

CRGB leds[NUM_LEDS];
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, MX_WIDTH, MX_HEIGHT,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

    
// тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка
#define __MODE_CANVAS 0
#define __MODE_TEXT   1
#define __MODE_GAME   2
unsigned char __CURRENT_MODE = __MODE_CANVAS;//__MODE_TEXT;

String _GAME_NAME_;
byte _GAME_BUTTONS_ = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

String _RUN_TEXT_ = "hello world";
int _RUN_TEXT_SPEED_ = 200;
uint32_t __RUNNING_STRING_COLOR = CRGB::Yellow;
CRGB __RUNNING_STRING_BACKGROUND_COLOR = CRGB::Blue;

#endif
