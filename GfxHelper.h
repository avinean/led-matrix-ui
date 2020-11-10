#ifndef _GFXHELPER_H_
#define _GFXHELPER_H_

#include "globals.h"

// Convert a BGR 4/4/4 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
    uint16_t RGB_bmp_fixed[w * h];
    for (uint16_t pixel=0; pixel<w*h; pixel++) {
  uint8_t r,g,b;
  uint16_t color = pgm_read_word(bitmap + pixel);

  //Serial.print(color, HEX);
  b = (color & 0xF00) >> 8;
  g = (color & 0x0F0) >> 4;
  r = color & 0x00F;
  //Serial.print(" ");
  //Serial.print(b);
  //Serial.print("/");
  //Serial.print(g);
  //Serial.print("/");
  //Serial.print(r);
  //Serial.print(" -> ");
  // expand from 4/4/4 bits per color to 5/6/5
  b = map(b, 0, 15, 0, 31);
  g = map(g, 0, 15, 0, 63);
  r = map(r, 0, 15, 0, 31);
  //Serial.print(r);
  //Serial.print("/");
  //Serial.print(g);
  //Serial.print("/");
  //Serial.print(b);
  RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
  //Serial.print(" -> ");
  //Serial.println(RGB_bmp_fixed[pixel], HEX);
    }
    matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}

// In a case of a tile of neomatrices, this test is helpful to make sure that the
// pixels are all in sequence (to check your wiring order and the tile options you
// gave to the constructor).
void count_pixels() {
    matrix->clear();
    for (uint16_t i=0; i<MX_HEIGHT; i++) {
  for (uint16_t j=0; j<MX_WIDTH; j++) {
      matrix->drawPixel(j, i, i%3==0?(uint16_t)LED_BLUE_HIGH:i%3==1?(uint16_t)LED_RED_HIGH:(uint16_t)LED_GREEN_HIGH);
      // depending on the matrix size, it's too slow to display each pixel, so
      // make the scan init faster. This will however be too fast on a small matrix.
      #ifdef ESP8266
      if (!(j%3)) matrix->show();
      yield(); // reset watchdog timer
      #elif ESP32
      delay(1);
      matrix->show();
      #else 
      matrix->show();
      #endif
  }
    }
}

// Fill the screen with multiple levels of white to gauge the quality
void display_four_white() {
    matrix->clear();
    matrix->fillRect(0,0, MX_WIDTH,MX_HEIGHT, LED_WHITE_HIGH);
    matrix->drawRect(1,1, MX_WIDTH-2,MX_HEIGHT-2, LED_WHITE_MEDIUM);
    matrix->drawRect(2,2, MX_WIDTH-4,MX_HEIGHT-4, LED_WHITE_LOW);
    matrix->drawRect(3,3, MX_WIDTH-6,MX_HEIGHT-6, LED_WHITE_VERYLOW);
    matrix->show();
}

void display_bitmap(uint8_t bmp_num, uint16_t color) { 
    static uint16_t bmx,bmy;

    // Clear the space under the bitmap that will be drawn as
    // drawing a single color pixmap does not write over pixels
    // that are nul, and leaves the data that was underneath
    matrix->fillRect(bmx,bmy, bmx+8,bmy+8, LED_BLACK);
    matrix->drawBitmap(bmx, bmy, mono_bmp[bmp_num], 8, 8, color);
    bmx += 8;
    if (bmx >= MX_WIDTH) bmx = 0;
    if (!bmx) bmy += 8;
    if (bmy >= MX_HEIGHT) bmy = 0;
    matrix->show();
}

void display_rgbBitmap(uint8_t bmp_num) { 
    static uint16_t bmx,bmy;

    fixdrawRGBBitmap(bmx, bmy, RGB_bmp[bmp_num], 8, 8);
    bmx += 8;
    if (bmx >= MX_WIDTH) bmx = 0;
    if (!bmx) bmy += 8;
    if (bmy >= MX_HEIGHT) bmy = 0;
    matrix->show();
}

void display_lines() {
    matrix->clear();

    // 4 levels of crossing red lines.
    matrix->drawLine(0,MX_HEIGHT/2-2, MX_WIDTH-1,2, LED_RED_VERYLOW);
    matrix->drawLine(0,MX_HEIGHT/2-1, MX_WIDTH-1,3, LED_RED_LOW);
    matrix->drawLine(0,MX_HEIGHT/2,   MX_WIDTH-1,MX_HEIGHT/2, LED_RED_MEDIUM);
    matrix->drawLine(0,MX_HEIGHT/2+1, MX_WIDTH-1,MX_HEIGHT/2+1, LED_RED_HIGH);

    // 4 levels of crossing green lines.
    matrix->drawLine(MX_WIDTH/2-2, 0, MX_WIDTH/2-2, MX_HEIGHT-1, LED_GREEN_VERYLOW);
    matrix->drawLine(MX_WIDTH/2-1, 0, MX_WIDTH/2-1, MX_HEIGHT-1, LED_GREEN_LOW);
    matrix->drawLine(MX_WIDTH/2+0, 0, MX_WIDTH/2+0, MX_HEIGHT-1, LED_GREEN_MEDIUM);
    matrix->drawLine(MX_WIDTH/2+1, 0, MX_WIDTH/2+1, MX_HEIGHT-1, LED_GREEN_HIGH);

    // Diagonal blue line.
    matrix->drawLine(0,0, MX_WIDTH-1,MX_HEIGHT-1, LED_BLUE_HIGH);
    matrix->drawLine(0,MX_HEIGHT-1, MX_WIDTH-1,0, LED_ORANGE_MEDIUM);
    matrix->show();
}

void display_boxes() {
    matrix->clear();
    matrix->drawRect(0,0, MX_WIDTH,MX_HEIGHT, LED_BLUE_HIGH);
    matrix->drawRect(1,1, MX_WIDTH-2,MX_HEIGHT-2, LED_GREEN_MEDIUM);
    matrix->fillRect(2,2, MX_WIDTH-4,MX_HEIGHT-4, LED_RED_HIGH);
    matrix->fillRect(3,3, MX_WIDTH-6,MX_HEIGHT-6, LED_ORANGE_MEDIUM);
    matrix->show();
}

void display_circles() {
    matrix->clear();
    matrix->drawCircle(MX_WIDTH/2,MX_HEIGHT/2, 2, LED_RED_MEDIUM);
    matrix->drawCircle(MX_WIDTH/2-1-min(MX_WIDTH,MX_HEIGHT)/8, MX_HEIGHT/2-1-min(MX_WIDTH,MX_HEIGHT)/8, min(MX_WIDTH,MX_HEIGHT)/4, LED_BLUE_HIGH);
    matrix->drawCircle(MX_WIDTH/2+1+min(MX_WIDTH,MX_HEIGHT)/8, MX_HEIGHT/2+1+min(MX_WIDTH,MX_HEIGHT)/8, min(MX_WIDTH,MX_HEIGHT)/4-1, LED_ORANGE_MEDIUM);
    matrix->drawCircle(1,MX_HEIGHT-2, 1, LED_GREEN_LOW);
    matrix->drawCircle(MX_WIDTH-2,1, 1, LED_GREEN_HIGH);
    if (min(MX_WIDTH,MX_HEIGHT)>12) matrix->drawCircle(MX_WIDTH/2-1, MX_HEIGHT/2-1, min(MX_HEIGHT/2-1,MX_WIDTH/2-1), LED_CYAN_HIGH);
    matrix->show();
}

void display_resolution() {
    matrix->setTextSize(1);
    // not wide enough;
    if (MX_WIDTH<16) return;
    matrix->clear();
    // Font is 5x7, if display is too small
    // 8 can only display 1 char
    // 16 can almost display 3 chars
    // 24 can display 4 chars
    // 32 can display 5 chars
    matrix->setCursor(0, 0);
    matrix->setTextColor(matrix->Color(255,0,0));
    if (MX_WIDTH>10) matrix->print(MX_WIDTH/10);
    matrix->setTextColor(matrix->Color(255,128,0)); 
    matrix->print(MX_WIDTH % 10);
    matrix->setTextColor(matrix->Color(0,255,0));
    matrix->print('x');
    // not wide enough to print 5 chars, go to next line
    if (MX_WIDTH<25) {
  if (MX_HEIGHT==13) matrix->setCursor(6, 7);
  else if (MX_HEIGHT>=13) {
      matrix->setCursor(MX_WIDTH-11, 8);
  } else {
      // we're not tall enough either, so we wait and display
      // the 2nd value on top.
      matrix->show();
      delay(2000);
      matrix->clear();
      matrix->setCursor(MX_WIDTH-11, 0);
  }   
    }
    matrix->setTextColor(matrix->Color(0,255,128)); 
    matrix->print(MX_HEIGHT/10);
    matrix->setTextColor(matrix->Color(0,128,255));  
    matrix->print(MX_HEIGHT % 10);
    // enough room for a 2nd line
    if ((MX_WIDTH>25 && MX_HEIGHT >14) || MX_HEIGHT>16) {
  matrix->setCursor(0, MX_HEIGHT-7);
  matrix->setTextColor(matrix->Color(0,255,255)); 
  if (MX_WIDTH>16) matrix->print('*');
  matrix->setTextColor(matrix->Color(255,0,0)); 
  matrix->print('R');
  matrix->setTextColor(matrix->Color(0,255,0));
  matrix->print('G');
  matrix->setTextColor(matrix->Color(0,0,255)); 
  matrix->print("B");
  matrix->setTextColor(matrix->Color(255,255,0)); 
  // this one could be displayed off screen, but we don't care :)
  matrix->print("*");

  // We have a big array, great, let's assume 32x32 and add something in the middle
  if (MX_HEIGHT>24 && MX_WIDTH>25) {
      for (uint16_t i=0; i<MX_WIDTH; i+=8) fixdrawRGBBitmap(i, MX_HEIGHT/2-7+(i%16)/8*6, RGB_bmp[10], 8, 8);
  }
    }
    
    matrix->show();
}



void display_scrollText(const char* txt[], const CRGB tcolor, const CRGB bgcolor) {
  uint8_t size = max(int(MX_WIDTH/8), 1);
  matrix->clear();
  matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
//  matrix->setTextSize(1);
  matrix->setRotation(0);
//  for (int8_t x=7; x>=-42; x--) {
//    yield();
//    matrix->clear();
//    matrix->setCursor(x,0);
//    matrix->setTextColor(LED_GREEN_HIGH);
//    matrix->print("Hello");
//    if (MX_HEIGHT>11) {
//      matrix->setCursor(-20-x,MX_HEIGHT-7);
//      matrix->setTextColor(LED_ORANGE_HIGH);
//      matrix->print("World");
//    }
//    matrix->show();
//    delay(50);
//  }

//  matrix->setRotation(3);
  matrix->setTextSize(size);
//  matrix->setTextColor(rgb888toRgb565(tcolor));
//  FastLED.setTextColor(tcolor);
uint16_t c16b = matrix->Color(tcolor.r, tcolor.g, tcolor.b);
uint16_t c16bbg = matrix->Color(bgcolor.r, bgcolor.g, bgcolor.b);
Serial.printf("CGRB color: %2X:%2X:%2X\n", tcolor.r, tcolor.g, tcolor.b);
Serial.printf("rgb565 color: %4X\n", c16b);
Serial.printf("CGRB BG color: %2X:%2X:%2X\n", bgcolor.r, bgcolor.g, bgcolor.b);
Serial.printf("rgb565 BG color: %4X\n\n", c16bbg);
matrix->setTextColor(c16b, c16bbg);
  for (int16_t x=8*size; x>=-6*8*size-MX_WIDTH*2; x--) {
    yield();
    matrix->clear();
    matrix->setCursor(x,MX_WIDTH/2-size*4);
    matrix->print((const char*)txt);
    matrix->show();
    // note that on a big array the refresh rate from show() will be slow enough that
    // the delay become irrelevant. This is already true on a 32x32 array.
//    delay(50/size);
//    vTaskDelay( pdMS_TO_TICKS( 50/size ) );
    vTaskDelay( pdMS_TO_TICKS( 50 ) );
  }
//  matrix->setRotation(0);
//  matrix->setCursor(0,0);
//  matrix->show();
}

void scrollTextTaskCode( void * pvParameters ){
  matrix->clear();
  matrix->show();
  for(;;){
    yield();
    display_scrollText((const char**)_RUN_TEXT_.c_str(), __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
    vTaskDelay( pdMS_TO_TICKS( 50 ) );
  }
}



uint8_t bitmapIdx = 0;

//void drawNextBitmap(int16_t x, int16_t y){
//  bitmapIdx = ( ++bitmapIdx > bitmapsCount ? 0 : bitmapsCount );
//  display_panOrBounceBitmap(32);
//}




// Scroll within big bitmap so that all if it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
void display_panOrBounceBitmap (volatile const bitmapInfo* bitmapNfo) {
  uint8_t bitmapSize = bitmapNfo->width;
    // keep integer math, deal with values 16 times too big
    // start by showing upper left of big bitmap or centering if the display is big
    int16_t xf = max(0, (MX_WIDTH-bitmapSize)/2) << 4;
    int16_t yf = max(0, (MX_HEIGHT-bitmapSize)/2) << 4;
    // scroll speed in 1/16th
    int16_t xfc = 6;
    int16_t yfc = 3;
    // scroll down and right by moving upper left corner off screen 
    // more up and left (which means negative numbers)
    int16_t xfdir = -1;
    int16_t yfdir = -1;

    for (uint16_t i=1; i<200; i++) {
  bool updDir = false;

  // Get actual x/y by dividing by 16.
  int16_t x = xf >> 4;
  int16_t y = yf >> 4;

  matrix->clear();

//(short unsigned int*)bitmapsData[0].bitmap
matrix->drawRGBBitmap(x, y, bitmapNfo->bitmap, bitmapSize, bitmapSize);
//  
//  // bounce 8x8 tri color smiley face around the screen
//  if (bitmapSize == 8) fixdrawRGBBitmap(x, y, RGB_bmp[10], 8, 8);
//  // pan 24x24 pixmap
//  if (bitmapSize == 24) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap24, bitmapSize, bitmapSize);
////#ifdef BM32
//  if (bitmapSize == 32) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap32, bitmapSize, bitmapSize);
////#endif
////  matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap32, 32, 32);
  matrix->show();
   
  // Only pan if the display size is smaller than the pixmap
  // but not if the difference is too small or it'll look bad.
  if (bitmapSize-(MX_WIDTH % 2 )>2) {
      xf += xfc*xfdir;
      if (xf >= 0)                      { xfdir = -1; updDir = true ; };
      // we don't go negative past right corner, go back positive
      if (xf <= (((MX_WIDTH % 2 )-bitmapSize) << 4)) { xfdir = 1;  updDir = true ; };
  }
  if (bitmapSize-( MX_HEIGHT % 2 )>2) {
      yf += yfc*yfdir;
      // we shouldn't display past left corner, reverse direction.
      if (yf >= 0)                      { yfdir = -1; updDir = true ; };
      if (yf <= ((( MX_HEIGHT % 2 )-bitmapSize) << 4)) { yfdir = 1;  updDir = true ; };
  }
  // only bounce a pixmap if it's smaller than the display size
  if ((2*MX_WIDTH)>bitmapSize) {
      xf += xfc*xfdir;
      // Deal with bouncing off the 'walls'
      if (xf >= ((2*MX_WIDTH)-bitmapSize) << 4) { xfdir = -1; updDir = true ; };
      if (xf <= 0)           { xfdir =  1; updDir = true ; };
  }
  if ((2*MX_HEIGHT)>bitmapSize) {
      yf += yfc*yfdir;
      if (yf >= ((2*MX_HEIGHT)-bitmapSize) << 4) { yfdir = -1; updDir = true ; };
      if (yf <= 0)           { yfdir =  1; updDir = true ; };
  }
  
  if (updDir) {
      // Add -1, 0 or 1 but bind result to 1 to 1.
      // Let's take 3 is a minimum speed, otherwise it's too slow.
      xfc = constrain(xfc + random(-1, 2), 3, 16);
      yfc = constrain(xfc + random(-1, 2), 3, 16);
  }
//  delay(100);
  vTaskDelay( pdMS_TO_TICKS( 100 ) );
    }
}

void drawTaskCode( void * pvParameters ){
  for(;;){
    yield();
    display_panOrBounceBitmap(&drawTaskBitmapInfo);
    vTaskDelay( pdMS_TO_TICKS( 50 ) );
  }
}

//void startDrawTask(){
//  TaskHandle_t xTask = currentGfxTask;
//
//  vTaskSuspendAll();  
//  if( currentGfxTask != NULL ){
//      /* The task is going to be deleted.
//      Set the handle to NULL. */
//      currentGfxTask = NULL;
//  
//      /* Delete using the copy of the handle. */
//      vTaskDelete( xTask );
//  }  
//
//drawTaskBitmapInfo = (bitmapInfo){ (const short unsigned int*)&drawTaskBitmapBuffer, 16, 16, 0 };
//
//  xTaskCreatePinnedToCore(
//    drawTaskCode,   /* Task function. */
//    "scrollText",     /* name of task. */
//    10000,       /* Stack size of task */
//    NULL,        /* parameter of the task */
//    2,           /* priority of the task */
//    &currentGfxTask,      /* Task handle to keep track of created task */
//    1);          /* pin task to core 0 */                  
//    xTaskResumeAll();
//}


void doLoop(){
      // clear the screen after X bitmaps have been displayed and we
    // loop back to the top left corner
    // 8x8 => 1, 16x8 => 2, 17x9 => 6
    static uint8_t pixmap_count = ((MX_WIDTH+7)/8) * ((MX_HEIGHT+7)/8);

// You can't use millis to time frame fresh rate because it uses cli() which breaks millis()
// So I use my stopwatch to count 200 displays and that's good enough
#if 0
    // 200 displays in 13 seconds = 15 frames per second for 4096 pixels
    for (uint8_t i=0; i<100; i++) { 
  matrix->fillScreen(LED_BLUE_LOW);
  matrix->show();
  matrix->fillScreen(LED_RED_LOW);
  matrix->show();
    }
#endif

    Serial.println("Count pixels");
    count_pixels();
    Serial.println("Count pixels done");
    delay(1000);

    display_four_white();
    delay(3000);

    Serial.print("Screen pixmap capacity: ");
    Serial.println(pixmap_count);

    // multicolor bitmap sent as many times as we can display an 8x8 pixmap
    for (uint8_t i=0; i<=pixmap_count; i++){
      display_rgbBitmap(0);
    }
    delay(1000);

    Serial.println("Display Resolution");
    display_resolution();
    delay(3000);

    Serial.println("Display bitmaps");
    // Cycle through red, green, blue, display 2 checkered patterns
    // useful to debug some screen types and alignment.
    uint16_t bmpcolor[] = { LED_GREEN_HIGH, LED_BLUE_HIGH, LED_RED_HIGH };
    for (uint8_t i=0; i<3; i++){
      display_bitmap(0, bmpcolor[i]);
      delay(500);
      display_bitmap(1, bmpcolor[i]);
      delay(500);
    }

    Serial.println("Display smileys");
    // Display 3 smiley faces.
    for (uint8_t i=2; i<=4; i++){
      display_bitmap(i, bmpcolor[i-2]);
      // If more than one pixmap displayed per screen, display more quickly.
      delay(MX_WIDTH>8?500:1500);
    }
    // If we have multiple pixmaps displayed at once, wait a bit longer on the last.
    delay(MX_WIDTH>8?1000:500);

    Serial.println("Display lines, boxes and circles");
    display_lines();
    delay(3000);

    display_boxes();
    delay(3000);

    display_circles();
    delay(3000);
    matrix->clear();

    Serial.println("Display RGB bitmaps");
    for (uint8_t i=0; i<=(sizeof(RGB_bmp)/sizeof(RGB_bmp[0])-1); i++){
      display_rgbBitmap(i);
      delay(MX_WIDTH>8?500:1500);
    }
    // If we have multiple pixmaps displayed at once, wait a bit longer on the last.
    delay(MX_WIDTH>8?1000:500);

    Serial.println("Scrolltext");
    display_scrollText((const char**)&"Scrolltext", CRGB::Red, CRGB::Black);


//drawNextBitmap();
bitmapInfo* itm = &bitmapsData[bitmapIdx];
Serial.printf("bounce bitmap[%d]: %dx%d", bitmapIdx, itm->width, itm->height);
bitmapIdx = ( ++bitmapIdx >= bitmapsCount ? 0 : bitmapIdx );
display_panOrBounceBitmap(&bitmapsData[bitmapIdx]);

//#ifdef BM32
//    Serial.println("bounce 32 bitmap");
//    display_panOrBounceBitmap(32);
//#endif
//    // pan a big pixmap
//    Serial.println("pan/bounce 24 bitmap");
//    display_panOrBounceBitmap(24);
//    // bounce around a small one
//    Serial.println("pan/bounce 8 bitmap");
//    display_panOrBounceBitmap(8);

}

void initGfx(){
  //#if defined(P32BY8X3)
//    // Parallel output
//    FastLED.addLeds<WS2811_PORTA,3>(leds, NUM_LEDS/3).setCorrection(TypicalLEDStrip);
//    Serial.print("Setup parrallel WS2811_PORTA: ");
//    Serial.print(NUM_LEDS);
//#else
//    FastLED.addLeds<NEOPIXEL,PIN>(  leds, NUM_LEDS  ).setCorrection(TypicalLEDStrip);
//    Serial.print("Setup serial: ");
//    Serial.println(NUM_LEDS);
//#endif
  FastLED.addLeds<WS2812, 4, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
//  FastLED.setBrightness( BRIGHTNESS );
  Serial.print("Matrix Size: ");
  Serial.print(MX_WIDTH);
  Serial.print(" ");
  Serial.print(MX_HEIGHT);
  Serial.print(" ");
  Serial.println(NUM_LEDS);

    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(BRIGHTNESS);
    Serial.println("If the code crashes here, decrease the brightness or turn off the all white display below");
    // Test full bright of all LEDs. If brightness is too high
    // for your current limit (i.e. USB), decrease it.
//#define DISABLE_WHITE
//#ifndef DISABLE_WHITE
//    matrix->fillScreen(LED_WHITE_HIGH);
//    matrix->show();
//    delay(3000);
//    matrix->clear();
//#endif
}

#endif
