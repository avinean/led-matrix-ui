#ifndef _GAMESHELPER_H_
#define _GAMESHELPER_H_

//#include "g_snake.h"

#include "Life.h"
Life life;

#include "Snake.h"
Snake snake;
//#include "g_arkanoid.h_"
#define DEBUG 0

Game* _games[] = {
        &life,
        &snake
};
Game * _game_pattern = _games[0];




void setupGames(){
//  __GAMES__[0] = funcPtrInfoBld( "Snake", 0, &snakeRoutine );
//  __GAMES__[1] = funcPtrInfoBld( "Arkanoid", 0, &arkanoidRoutine );
}

void gameTimerHandler( TimerHandle_t xTimer ){
#if DEBUG == 1
    Serial.printf("\ngameTimerHandler\n");
#endif
    portENTER_CRITICAL(&_game_pattern->timerMux);
    _game_pattern->gameTimerisReady = true;
    portEXIT_CRITICAL(&_game_pattern->timerMux);
    ESP_LOGI(TAG,"tring tring!!!");
//    snakeRoutine();
//    matrix->show();
}

void gameStart(){
    _game_pattern->start();
    startGameTimer(gameTimerHandler, _game_pattern->interval, 1);
}

void gameRoutine(){
//  _GAME_NAME_
    _game_pattern->drawFrame();
    matrix->show();
}

#endif
