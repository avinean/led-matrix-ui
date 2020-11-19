#ifndef _GAMESHELPER_H_
#define _GAMESHELPER_H_

#include "g_snake.h"
#include "g_arkanoid.h"


void setupGames(){
  __GAMES__[0] = funcPtrInfoBld( "Snake", 0, &snakeRoutine );
  __GAMES__[1] = funcPtrInfoBld( "Arkanoid", 0, &arkanoidRoutine );
}


void gameRoutine(){
//  _GAME_NAME_
}

#endif
