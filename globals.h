#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "timerMinim.h"




unsigned char bitmap[BITMAP_SIZE];


bool _runningStringActive = false;
String _runningString = "hello world";
int _runningStringSpeed = 200;
timerMinim scrollTimer(_runningStringSpeed);                   // Таймер прокрутки текста эффекта бегущей строки
timerMinim gameTimer(DEMO_GAME_SPEED);                  // Таймер скорости игры (шага выполнения игры)


#endif //_GLOBALS_H_
