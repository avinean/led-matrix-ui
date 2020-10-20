#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "timerMinim.h"
#include <FastLED.h>

IPAddress myIP;

unsigned char bitmap[BITMAP_SIZE];

#define __MODE_CANVAS 0
#define __MODE_TEXT   1
#define __MODE_GAME   2

// тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка
unsigned char __CURRENT_MODE = __MODE_CANVAS;//__MODE_TEXT;


boolean loadingFlag = true;        // флаг: выполняется инициализация параметров режима

#define GAME_NAME_SIZE 10
String _GAME_NAME_;//[GAME_NAME_SIZE];
// включение / отключение игр

boolean gamemodeFlag = false;      // флаг: текущий режим - игра
boolean controlFlag = false;       // флаг: управление игрой перехвачено с кнопок или смартфона
boolean gamePaused;                // флаг: игра приостановлена
boolean gameDemo = true;           // флаг: игра в демо-режиме
#define DEMO_GAME_SPEED 60    // скорость игр в демо режиме (мс)

byte frameNum;                     // Номер фрейма проигрывания анимации
byte buttons = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата


String _runningString = "hello world";
int _runningStringSpeed = 200;
uint32_t __RUNNING_STRING_COLOR = CRGB::Yellow;
CRGB __RUNNING_STRING_BACKGROUND_COLOR = CRGB::Blue;
boolean fullTextFlag = false;      // флаг: текст бегущей строки показан полностью (строка убежала)
timerMinim scrollTimer(0);                   // Таймер прокрутки текста эффекта бегущей строки
timerMinim gameTimer(0);                  // Таймер скорости игры (шага выполнения игры)


#endif //_GLOBALS_H_
