/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GAME_H
#define GAME_H

//#include "matrix.h"
#include "helpers.h"
//#include "../.pio/libdeps/esp32dev/FastLED/pixeltypes.h"
//#include "g_snake.h"
#include "matrixHelper.h"


class Game{
public:
    char* name;
    uint16_t score;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
    bool gameTimerisReady = false;



    // a single frame should be drawn as fast as possible, without any delay or blocking
    // return how many millisecond delay is requested before the next call to drawFrame()
    virtual unsigned int drawFrame() {
	// not visible in this file (defined in neomatrix_config.h)
        //backgroundLayer.fillScreen({ 0, 0, 0 });
        return 0;
    };

//    virtual void routine() {};

    virtual boolean checkButtons() {
        if (_GAME_BUTTONS_ != 4) return true;
        return false;
    }

    virtual void buttonsTick(){};

//    virtual void gameTimerHandler( TimerHandle_t xTimer ){}

    // отображаем счёт для игр
    virtual void displayScore(byte score) {
        /*matrix->clear();
        matrix->setCursor(MX_WIDTH / 2 - 4, MX_HEIGHT / 2 - 3);
        matrix->setTextColor(GLOBAL_COLOR_1);//, c16bbg);
        matrix->print(score / 10);
        matrix->setCursor(MX_WIDTH / 2, MX_HEIGHT / 2 - 3);
        matrix->setTextColor(GLOBAL_COLOR_2);//, c16bbg);
        matrix->print(score % 10);
        matrix->show();
        delay(1000);*/
    }

    virtual void newGame(){};

    virtual void start() {};
    virtual void stop() {};
    uint16_t interval;
    bool gameDemo = true;
};

#endif
