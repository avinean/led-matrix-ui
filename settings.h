#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// SSID & Password
const char* ssid = "AndroidAP";//"Keenetic-1933";  // Enter your SSID here
const char* password = "01234567";//"sMTVCwBP";  //Enter your Password here

// Set these to your desired credentials.
const char *Apssid = "dreamer-led-AP";     //Give AccessPoint name whatever you like. (this will be Name of your esp32 HOTSPOT)
const char *Appassword = "01234567";         //Password of your Esp32's hotspot,(minimum length 8 required)

#define USE_FONTS 1
#define USE_SNAKE 1         // игра змейка (0 нет, 1 да)



// о поддерживаемых цветах читай тут https://alexgyver.ru/gyvermatrixos-guide/
#define GLOBAL_COLOR_1 CRGB::Green    // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange   // основной цвет №2 для игр

#define BG_COLOR_THRESH_MAX 250


#define MX_WIDTH  16
#define MX_HEIGHT 16
#define DEPTH  3
#define NUM_LEDS ( MX_WIDTH * MX_HEIGHT )
#define BITMAP_SIZE ( NUM_LEDS * DEPTH )

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;


#define MATRIX_TYPE 0       // тип матрицы: 0 - зигзаг, 1 - последовательная
#define CONNECTION_ANGLE 0  // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 3   // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#define LED_PIN  4

#define COLOR_ORDER GRB
#define CHIPSET     WS2812

#define BRIGHTNESS 4 

// **************** НАСТРОЙКА МАТРИЦЫ ****************
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH MX_WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH MX_HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH MX_WIDTH
#define THIS_X x
#define THIS_Y (MX_HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH MX_HEIGHT
#define THIS_X (MX_HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH MX_WIDTH
#define THIS_X (MX_WIDTH - x - 1)
#define THIS_Y (MX_HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH MX_HEIGHT
#define THIS_X (MX_HEIGHT - y - 1)
#define THIS_Y (MX_WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH MX_WIDTH
#define THIS_X (MX_WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH MX_HEIGHT
#define THIS_X y
#define THIS_Y (MX_WIDTH - x - 1)

#else
#define _WIDTH MX_WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif




#define D_TEXT_SPEED 100      // скорость бегущего текста по умолчанию (мс)
#define D_TEXT_SPEED_MIN 10
#define D_TEXT_SPEED_MAX 255





byte modeCode;                     // тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка,
int8_t thisMode = 0;               // текущий режим


#endif // _SETTINGS_H_
