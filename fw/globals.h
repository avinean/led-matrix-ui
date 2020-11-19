#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defs.h"
#include "timerMin.h"





const char effectsCount = 13;
funcPtrInfo __EFFECTS__[effectsCount];

const char gamesCount = 2;
funcPtrInfo __GAMES__[gamesCount];

//uint16_t _buff[BITMAP_SIZE*4];
// for jpg helper
uint16_t __w = 0, __h = 0, __scale;
String __imgName;

const char* host = "dreamer-led";
const char* ssid = "Keenetic-1933";
const char* password = "sMTVCwBP";


// LED pins
const int LED_BUILTIN = 2;


String __GIFS_FOLDER = "/gifs";
String __JPGS_FOLDER = "/imgs";

volatile uint16_t __SPEED__ = 300; // скорость анимаций ( точнее - delay() после отрисовки очередного кадра, или скорость игр в демо режиме (мс)
volatile bool __MODE_STOP__ = true;

#define USE_SNAKE 1         // игра змейка (0 нет, 1 да)
volatile boolean loadingFlag = true;        // флаг: выполняется инициализация параметров режима

boolean gamemodeFlag = false;      // флаг: текущий режим - игра
boolean controlFlag = false;       // флаг: управление игрой перехвачено с кнопок или смартфона
boolean gamePaused;                // флаг: игра приостановлена
boolean gameDemo = true;           // флаг: игра в демо-режиме

byte frameNum;                     // Номер фрейма проигрывания анимации
//byte buttons = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

// о поддерживаемых цветах читай тут https://alexgyver.ru/gyvermatrixos-guide/
#define GLOBAL_COLOR_1 CRGB::Green    // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange   // основной цвет №2 для игр


#define GAME_NAME_SIZE 10
String _GAME_NAME_;//[GAME_NAME_SIZE];

timerMinim gameTimer(0);                  // Таймер скорости игры (шага выполнения игры)

uint8_t _EFFECT_ID = 0;



bitmapInfo drawTaskBitmapInfo;
//uint8_t drawTaskBitmapBuffer[BITMAP_SIZE*4];
uint16_t drawTaskBitmapBuffer[NUM_LEDS*4];


//const String CONFIG_IDF_TARGET = "esp32";

#include <ESPAsyncWebServer.h>
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/echo");


//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
CRGB *matrixleds = (CRGB*)&leds;
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MX_WIDTH, MX_HEIGHT,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

uint8_t _MX_ROTATION = 0;
char __BOOT_STR__[100];

#include "effects/Drawable.h"
#include "effects/Effects.h"
Effects effects;

    
// тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка
#define __MODE_BOOT     -1
#define __MODE_CANVAS   0
#define __MODE_TEXT     1
#define __MODE_GAME     2
#define __MODE_GIF      3
#define __MODE_GIF_PLAY 4
#define __MODE_JPG      5
#define __MODE_JPG_PLAY 6
#define __MODE_EFFECTS  7
#define __MODE_DRAW     8
unsigned char __CURRENT_MODE = __MODE_CANVAS;//__MODE_TEXT;

//volatile 
//String _GAME_NAME_;
byte _GAME_BUTTONS_ = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

//volatile 
String _RUN_TEXT_ = "hello world";

CRGB __RUNNING_STRING_COLOR = CRGB::Yellow;
CRGB __RUNNING_STRING_BACKGROUND_COLOR = CRGB::Blue;

#endif
