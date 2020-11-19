#ifndef _GARKANOID_H_
#define _GARKANOID_H_


// игра "arkanoid"

#define USE_ARKAN 1

#include "helpers.h"

// **************** НАСТРОЙКИ ARKAN ****************
#define SHELF_LENGTH 5    // длина полки
#define VELOCITY 5        // скорость шара
#define BALL_SPEED 50     // период интегрирования

#define BLOCKS_H 4        // высота кучи блоков
#define LINE_NUM 8        // количество "линий" с блоками других уровней
#define LINE_MAX 4        // макс. длина линии

// цвета блоков по крутости
#define BLOCK_COLOR_1 CRGB::Aqua
#define BLOCK_COLOR_2 CRGB::Amethyst
#define BLOCK_COLOR_3 CRGB::Blue

#if (USE_ARKAN == 1)
int posX_ark;
int posY_ark;
int8_t velX_ark = 3;
int8_t velY_ark = (long)sqrt(sq(VELOCITY) - sq(velX_ark));
int8_t shelf_x = MX_WIDTH / 2 - SHELF_LENGTH / 2;
byte shelfMAX = MX_WIDTH - SHELF_LENGTH;
int arkScore;
int lastSpeed;

void generateBlocks();

timerMinim popTimeout(500);
timerMinim shelfTimer(150);

void newGameArkan() {
  arkScore = 0;
  generateBlocks();
  shelf_x = MX_WIDTH / 2 - SHELF_LENGTH / 2;
  posX_ark = MX_WIDTH / 2 * 10;
  posY_ark = 15;
  velX_ark = random(1, 4);
  velY_ark = (long)sqrt(sq(VELOCITY) - sq(velX_ark));
}

boolean checkBlocks() {   // возвр ДА если блоков нет
  for (byte j = MX_HEIGHT - 1; j > MX_HEIGHT - 1 - BLOCKS_H; j--) {
    for (byte i = 0; i < MX_WIDTH; i++) {
      if (getPixColorXY(i, j) != BLOCK_COLOR_1) return false;
      else if (getPixColorXY(i, j) != BLOCK_COLOR_2) return false;
      else if (getPixColorXY(i, j) != BLOCK_COLOR_3) return false;
    }
  }
  return true;
}

void redrawBlock(uint16_t blockX, uint16_t blockY) {
  arkScore++;
  if (getPixColorXY(blockX, blockY) == BLOCK_COLOR_1) matrix->drawPixel(blockX, blockY, (uint16_t)0);
  else if (getPixColorXY(blockX, blockY) == BLOCK_COLOR_2) matrix->drawPixel(blockX, blockY, (uint16_t)BLOCK_COLOR_1);
  else if (getPixColorXY(blockX, blockY) == BLOCK_COLOR_3) matrix->drawPixel(blockX, blockY, (uint16_t)BLOCK_COLOR_2);
}

void generateBlocks() {
  for (byte j = MX_HEIGHT - 1; j > MX_HEIGHT - 1 - BLOCKS_H; j--) {
    for (byte i = 0; i < MX_WIDTH; i++) {
      matrix->drawPixel(i, j, (uint16_t)BLOCK_COLOR_1);
    }
  }
  for (byte k = 0; k < LINE_NUM; k++) {
    byte newPosX = random(0, MX_WIDTH - 1 - LINE_MAX);
    byte newPosY = random(MX_HEIGHT - BLOCKS_H, MX_HEIGHT);
    byte newColor = random(0, 3);
    for (byte i = newPosX; i < newPosX + LINE_MAX; i++) {
      switch (newColor) {
        case 0: matrix->drawPixel((uint16_t)i, (uint16_t)newPosY, (uint16_t)0);
          break;
        case 1: matrix->drawPixel((uint16_t)i, (uint16_t)newPosY, (uint16_t)BLOCK_COLOR_2);
          break;
        case 2: matrix->drawPixel((uint16_t)i, (uint16_t)newPosY, (uint16_t)BLOCK_COLOR_3);
          break;
      }
    }
  }
}

void gameOverArkan() {
  displayScore(arkScore);
  delay(800);
  FastLED.clear();
  newGameArkan();
}

void shelfRight() {
  shelf_x++;            // прибавить координату полки
  if (shelf_x > shelfMAX) { // если полка пробила правый край
    shelf_x = shelfMAX;
  } else {
    matrix->drawPixel(shelf_x - 1, 0, (uint16_t)CRGB::Black);   // стереть последнюю точку
    matrix->drawPixel(shelf_x + SHELF_LENGTH - 1, 0, (uint16_t)GLOBAL_COLOR_2);  // нарисовать первую
    FastLED.show();
  }
}

void shelfLeft() {
  shelf_x--;
  if (shelf_x < 0) { // если полка пробила левый край
    shelf_x = 0;
  } else {
    matrix->drawPixel(shelf_x, 0, (uint16_t)GLOBAL_COLOR_2);   // стереть последнюю точку
    matrix->drawPixel(shelf_x + SHELF_LENGTH, 0, (uint16_t)CRGB::Black);  // нарисовать первую
    FastLED.show();
  }
}


void arkanoidRoutine() {
  if (loadingFlag) {
    FastLED.clear();
    loadingFlag = false;
    gamemodeFlag = true;
    __CURRENT_MODE = __MODE_GAME;
    newGameArkan();
    FastLED.show();
  }

  if (gameDemo) {
    if (shelfTimer.isReady()) {
      if (floor(posX_ark / 10) > shelf_x - SHELF_LENGTH / 2 - 1) _GAME_BUTTONS_ = 1;
      if (floor(posX_ark / 10) < shelf_x + SHELF_LENGTH / 2 + 1) _GAME_BUTTONS_ = 3;
    }
  }

  if (checkButtons()) {
    if (_GAME_BUTTONS_ == 3) {   // кнопка нажата
      shelfLeft();
    }
    if (_GAME_BUTTONS_ == 1) {   // кнопка нажата
      shelfRight();
    }
    _GAME_BUTTONS_ = 4;
  }

  if (lastSpeed != __SPEED__) {
    lastSpeed = __SPEED__;
    gameTimer.setInterval(__SPEED__ / 2);
  }

  if (gameTimer.isReady()) {        // главный таймер игры
    matrix->drawPixel(posX_ark / 10, posY_ark / 10, (uint16_t)CRGB::Black);
    posX_ark = posX_ark + velX_ark;
    posY_ark = posY_ark + velY_ark;
    int8_t posX_arkm = posX_ark / 10;
    int8_t posY_arkm = posY_ark / 10;
    if (abs(velY_ark) <= 2) {
      velX_ark = 3;
      velY_ark = (long)sqrt(sq(VELOCITY) - sq(velX_ark));
    }

    // отскок левый край
    if (posX_arkm < 0) {
      posX_ark = 0;
      velX_ark = -velX_ark;
    }

    // отскок правый край
    if (posX_arkm > MX_WIDTH - 1) {
      posX_ark = (MX_WIDTH - 1) * 10;
      velX_ark = -velX_ark;
    }

    // проверка на пробитие дна
    if (posY_ark < 9) {
      gameOverArkan();
      //posY_ark = 0;
      //velY_ark = -velY_ark;
    }

    // проверка на ударение с площадкой
    if (velY_ark < 0 && posY_ark > 10 && posY_ark <= 20 && posX_arkm >= shelf_x && posX_arkm < (shelf_x + SHELF_LENGTH)) {
      // таймаут, чтолбы исключить дрочку у полки
      if (popTimeout.isReady()) {

        // тут короч если длина полки больше двух, то её края "подкручивают"
        // шарик, т.е. при отскоке меняют скорость по оси Х
        if (SHELF_LENGTH > 2) {
          if (posX_arkm == shelf_x) {
            velX_ark -= 2;  // уменьшаем скорость по Х
            // расчёт скорости по У с учётом общего заданного вектора скорости
            velY_ark = (long)sqrt(sq(VELOCITY) - sq(velX_ark));
          } else if (posX_arkm == (shelf_x + SHELF_LENGTH - 1)) {
            velX_ark += 2;  // увеличиваем скорость по Х
            velY_ark = (long)sqrt(sq(VELOCITY) - sq(velX_ark));
          } else {
            velY_ark = -velY_ark;
          }
        } else {
          velY_ark = -velY_ark;
        }
      }
    }

    // пробитие верха
    if (posY_arkm > MX_HEIGHT - 1) {
      posY_ark = (MX_HEIGHT - 1) * 10;
      velY_ark = -velY_ark;
    }
    byte ballX = floor(posX_ark / 10);
    byte ballY = floor(posY_ark / 10);

    if (ballY > 2) {
      if (ballX < MX_WIDTH - 1 && velX_ark > 0 && getPixColorXY(ballX + 1, ballY) != 0) {
        redrawBlock(ballX + 1, ballY);
        velX_ark = -velX_ark;
      }
      if (ballX > 1 && velX_ark < 0 && getPixColorXY(ballX - 1, ballY) != 0) {
        redrawBlock(ballX - 1, ballY);
        velX_ark = -velX_ark;
      }
      if (ballY < MX_HEIGHT - 1 && velY_ark > 0 && getPixColorXY(ballX, ballY + 1) != 0) {
        redrawBlock(ballX, ballY + 1);
        velY_ark = -velY_ark;
      }
      if (velY_ark < 0 && getPixColorXY(ballX, ballY - 1) != 0) {
        redrawBlock(ballX, ballY - 1);
        velY_ark = -velY_ark;
      }
    }

    if (checkBlocks()) gameOverArkan();    

    matrix->drawPixel(ballX, ballY, (uint16_t)GLOBAL_COLOR_1);
    for (byte i = shelf_x; i < shelf_x + SHELF_LENGTH; i++) {
      matrix->drawPixel(i, 0, (uint16_t)GLOBAL_COLOR_2);
    }
    FastLED.show();
  }
}


#else
void arkanoidRoutine() {
  return;
}
#endif

#endif
