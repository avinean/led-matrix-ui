#ifndef _GSNAKE_H_
#define _GSNAKE_H_

// игра змейка!
#if (USE_SNAKE == 1)

#include "helpers.h"

// **************** НАСТРОЙКИ ЗМЕЙКИ ****************
#define START_LENGTH 4    // начальная длина змейки
#define MAX_LENGTH 80     // максимальная длина змейки

// **************** ДЛЯ РАЗРАБОТЧИКОВ ****************
int8_t vectorX, vectorY;
int8_t headX, headY, buttX, buttY;
int8_t appleX, appleY;
boolean apple_flag, missDelete = false;

int8_t buttVector[MAX_LENGTH];
int snakeLength;
boolean butt_flag, pizdetc;

void newGameSnake();
void buttonsTickSnake();
void snakeDemo();

// отображаем счёт для игр
void displayScore(byte score) {
//  if (SCORE_SIZE == 0) {
//    if (score > 9) drawDigit3x5(score / 10, WIDTH / 2 - 4, HEIGHT / 2 - 3, GLOBAL_COLOR_1);
//    drawDigit3x5(score % 10, WIDTH / 2, HEIGHT / 2 - 3, GLOBAL_COLOR_2);
//  } else {
//    if (score > 9) drawDigit5x7(score / 10, WIDTH / 2 - 6, HEIGHT / 2 - 4, GLOBAL_COLOR_1);
//    drawDigit5x7(score % 10, WIDTH / 2, HEIGHT / 2 - 4, GLOBAL_COLOR_2);
//  }
//  FastLED.show();
}

void snakeRoutine() {
  if (loadingFlag) {
    FastLED.clear();
    loadingFlag = false;
    newGameSnake();
    gamemodeFlag = true;
    __CURRENT_MODE = __MODE_GAME;
  }  
  
  buttonsTickSnake();

  if (gameTimer.isReady()) {
    // БЛОК ГЕНЕРАЦИИ ЯБЛОКА
    while (!apple_flag) {                         // пока яблоко не создано
      appleX = random(0, MX_WIDTH);                  // взять случайные координаты
      appleY = random(0, MX_HEIGHT);

      // проверить, не совпадает ли координата с телом змеи
      if ((long)getPixColorXY(appleX, appleY) == 0) {
        apple_flag = true;                        // если не совпадает, считаем что яблоко создано
        matrix->drawPixel(appleX, appleY, (uint16_t)GLOBAL_COLOR_2 );        // и рисуем
        FastLED.show();
      }
    }

    // запоминаем, куда повернули голову
    // 0 - право, 1 - лево, 2 - вверх, 3 - вниз
    if (vectorX > 0) buttVector[snakeLength] = 0;
    else if (vectorX < 0) buttVector[snakeLength] = 1;
    if (vectorY > 0) buttVector[snakeLength] = 2;
    else if (vectorY < 0) buttVector[snakeLength] = 3;

    // смещение головы змеи
    headX += vectorX;
    headY += vectorY;

    if (headX < 0 || headX > MX_WIDTH - 1 || headY < 0 || headY > MX_HEIGHT - 1) { // если вышла за границы поля
      pizdetc = true;
    }

    if (!pizdetc) {
      // проверка на pizdetc
      if ((long)(getPixColorXY(headX, headY) != 0 && (long)getPixColorXY(headX, headY) != GLOBAL_COLOR_2)) {   // если змея врезалась во что то, но не в яблоко
        pizdetc = true;                           // флаг на отработку
      }

      // БЛОК ОТРАБОТКИ ПОЕДАНИЯ ЯБЛОКА
      if (!pizdetc && (long)getPixColorXY(headX, headY) == (long)GLOBAL_COLOR_2) { // если попали головой в яблоко
        apple_flag = false;                       // флаг что яблока больше нет
        snakeLength++;                            // увеличить длину змеи
        buttVector[snakeLength] = 4;              // запоминаем, что надо будет не стирать хвост
      }

      // вычисляем координату хвоста (чтобы стереть) по массиву вектора
      switch (buttVector[0]) {
        case 0: buttX += 1;
          break;
        case 1: buttX -= 1;
          break;
        case 2: buttY += 1;
          break;
        case 3: buttY -= 1;
          break;
        case 4: missDelete = true;  // 4 значит не стирать!
          break;
      }

      // смещаем весь массив векторов хвоста ВЛЕВО
      for (byte i = 0; i < snakeLength; i++) {
        buttVector[i] = buttVector[i + 1];
      }

      // если змея не в процессе роста, закрасить бывший хвост чёрным
      if (!missDelete) {
        matrix->drawPixel(buttX, buttY, (uint16_t)0x000000);
      }
      else missDelete = false;

      // рисуем голову змеи в новом положении
      matrix->drawPixel(headX, headY, (uint16_t)GLOBAL_COLOR_1);
      FastLED.show();
    }
    if (gameDemo) snakeDemo();
  }

  // если он настал
  if (pizdetc) {
    pizdetc = false;

    // ну в общем плавно моргнуть, типо змейке "больно"
    for (byte bright = 0; bright < 15; bright++) {
      FastLED.setBrightness(bright);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
      }
      FastLED.show();
      delay(10);
    }
    delay(100);
    FastLED.clear();
    FastLED.show();
    FastLED.setBrightness(BRIGHTNESS);
    displayScore(snakeLength - START_LENGTH);
    delay(1000);
    FastLED.clear();
    FastLED.show();
    newGameSnake();                          // миша, всё ху.я, давай по новой
  }
}

void snakeDemo() {
  // смещение головы змеи
  int8_t nextX = headX + vectorX;
  int8_t nextY = headY + vectorY;

  // ищем яблоко
  if (headX == appleX) {                // яблоко на одной линии по вертикали
    if (headY < appleY) _GAME_BUTTONS_ = 0;
    if (headY > appleY) _GAME_BUTTONS_ = 2;
  }
  if (headY == appleY) {                // яблоко на одной линии по горизонтали
    if (headX < appleX) _GAME_BUTTONS_ = 1;
    if (headX > appleX) _GAME_BUTTONS_ = 3;
  }

  if (getPixColorXY(nextX, nextY) == GLOBAL_COLOR_1) {   // проверка на столкновение с собой
    // поворачиваем налево
    if (vectorX > 0) _GAME_BUTTONS_ = 0;
    if (vectorX < 0) _GAME_BUTTONS_ = 2;
    if (vectorY > 0) _GAME_BUTTONS_ = 3;
    if (vectorY < 0) _GAME_BUTTONS_ = 1;
    return;
  }

  if (nextX < 0 || nextX > MX_WIDTH - 1 || nextY < 0        // проверка на столкновение со стеной
      || nextY > MX_HEIGHT - 1) {

    // поворачиваем направо в обычном случае или налево в углу
    if (vectorX > 0) _GAME_BUTTONS_ = 2;
    if (vectorX > 0 && headY == 0) _GAME_BUTTONS_ = 0;

    if (vectorX < 0 && headY == MX_HEIGHT - 1) _GAME_BUTTONS_ = 2;
    if (vectorX < 0) _GAME_BUTTONS_ = 0;

    if (vectorY > 0) _GAME_BUTTONS_ = 1;
    if (vectorY > 0 && headX == MX_WIDTH - 1) _GAME_BUTTONS_ = 3;

    if (vectorY < 0 && headX == 0) _GAME_BUTTONS_ = 1;
    if (vectorY < 0) _GAME_BUTTONS_ = 3;
  }
}

boolean checkButtons() {
  if (_GAME_BUTTONS_ != 4) return true;
  return false;
}

void buttonsTickSnake() {
  if (checkButtons()) {
    if (_GAME_BUTTONS_ == 3) {   // кнопка нажата
      vectorX = -1;
      vectorY = 0;
      _GAME_BUTTONS_ = 4;
    }
    if (_GAME_BUTTONS_ == 1) {   // кнопка нажата
      vectorX = 1;
      vectorY = 0;
      _GAME_BUTTONS_ = 4;
    }
    if (_GAME_BUTTONS_ == 0) {   // кнопка нажата
      vectorY = 1;
      vectorX = 0;
      _GAME_BUTTONS_ = 4;
    }
    if (_GAME_BUTTONS_ == 2) {   // кнопка нажата
      vectorY = -1;
      vectorX = 0;
      _GAME_BUTTONS_ = 4;
    }
  }
}

void newGameSnake() {
  FastLED.clear();
  // свежее зерно для генератора случайных чисел
  randomSeed(millis());

  // длина из настроек, начинаем в середине экрана, бла-бла-бла
  snakeLength = START_LENGTH;
  headX = MX_WIDTH / 2;
  headY = MX_HEIGHT / 2;
  buttY = headY;

  vectorX = 1;  // начальный вектор движения задаётся вот здесь
  vectorY = 0;
  _GAME_BUTTONS_ = 4;

  // первоначальная отрисовка змейки и забивка массива векторов для хвоста
  for (byte i = 0; i < snakeLength; i++) {
    matrix->drawPixel(headX - i, headY, (uint16_t)GLOBAL_COLOR_1);
    buttVector[i] = 0;
  }
  FastLED.show();
  buttX = headX - snakeLength;   // координата хвоста как голова - длина
  missDelete = false;
  apple_flag = false;
}

#elif (USE_SNAKE == 0)
void snakeRoutine() {
  return;
}
#endif

#endif
