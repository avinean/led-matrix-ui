#ifndef _JPGHELPER_H_
#define _JPGHELPER_H_

// JPEG decoder library
#include <TJpg_Decoder.h>

#include "soc/rtc_wdt.h"

#include "globals.h"



//====================================================================================
//                                    tft_output
//====================================================================================
// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{

//Serial.printf("x: %d, y: %d; w: %d, h: %d, bitmap: %d, bitmap_sz: %d\n", x, y, w, h, bitmap, sizeof(bitmap));
  
  // Stop further decoding as image is running off bottom of screen
  if ( y >= MX_HEIGHT ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
//  tft.pushImage(x, y, w, h, bitmap);
//matrix->clear();

uint16_t *bptr = bitmap;
uint16_t _max = -1;
uint16_t _min = 9999;



for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++) {
//      Serial.printf("i: %d, j: %d; x+i: %d, y+j: %d, minn: %d\n", i, j, x+i, y+j, -(2*i));
//      writePixel(x + i, y, bitmap[j * w + i]);
      uint16_t _t = bitmap[XY(i, j, w)];
      
      drawTaskBitmapBuffer[XY(x + i, y + j, ( __w >= MX_WIDTH / 2 ? MX_WIDTH : __w ) )] = _t;
    }
  }

//

  // Return 1 to decode next block
  return 1;
}

bool flip = true;

void drawJpeg(const char *filename, int xpos, int ypos, uint16_t* _buff) {
//  tft.fillScreen(TFT_RED);
  matrix->clear();  
  matrix->fillScreen(CRGB::Green);
  matrix->show();

  // Time recorded for test purposes
  uint32_t t = millis();

  // Get the width and height in pixels of the jpeg if you wish
  
  TJpgDec.getFsJpgSize(&__w, &__h, filename); // Note name preceded with "/"
//  tft.setRotation(w > h ? 1 : 0);
//Serial.print("Width = "); Serial.print(__w); Serial.print(", height = "); Serial.println(__h);

  for (__scale = 1; __scale <= 8; __scale <<= 1) {
    if (__w <= MX_WIDTH * __scale && __h <= MX_HEIGHT * __scale) break;
  }
  TJpgDec.setJpgScale(__scale);

  // Draw the image, top left at 0,0
  TJpgDec.drawFsJpg(0, 0, filename);

//matrix->drawRGBBitmap(0, 0, _buff, __w, __h); 
//display_panOrBounceBitmap(_buff); 

//static uint8_t hue;
//  for(int i = 0; i < NUM_LEDS/2; i++) {   
//    // fade everything out
//    leds.fadeToBlackBy(40);
//
//    // let's set an led value
//    leds[i] = CHSV(hue++,255,255);
//
//    // now, let's first 20 leds to the top 20 leds, 
//    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
//    FastLED.delay(33);
//  }

  

     for (int16_t j = 0; j < __h; j++) {
      for (int16_t i = 0; i < __w; i++) {      
        matrix->drawRGBBitmap(i, j, _buff+XY(i, j, __w), __w, 1); 

        uint16_t color = *_buff+XY(i, __h - j, __w);
        uint8_t r = ((color >> 11) & 0x1F);
        uint8_t g = ((color >> 5) & 0x3F);
        uint8_t b = (color & 0x1F);
        
//        matrix->drawPixel(i, j, gammaCorrection( matrix->Color( r,g,b ) ) );
//        matrix->drawRGBBitmap(__w - i, __h - j, _buff+XY(i, __h - j, __w), __w, 1); 
        rtc_wdt_feed();matrix->show();    
        delay(30);
    //    matrix->drawPixel(i, j, matrix->Color(0,0,0));
    //    matrix->show();    
    //    delay(10);
      }
    }

    

//    for (int16_t x = 0; x < __w; x++) {
//      for (int16_t y = 0; y < __h; y++) {      
//        uint32_t color = gammaCorrection( leds[XY(x,y)] );
//        uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
//        uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
//        uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;        
//        
//        matrix->drawPixel(x, y, matrix->Color( r,g,b ) );
//        matrix->show();    
//        delay(10);
//        
////        matrix->drawRGBBitmap(__w - i, __h - j, _buff+XY(i, __h - j, __w), __w, 1); 
//        
//      }
//    }
//    matrix->show();    
//    delay(3000);

/*
  matrix->clear();
  if ( flip ) {
    for (int16_t j = 0; j < __h / 2; j++) {  
      for (int16_t i = 0; i < __w; i++) {      
        matrix->drawRGBBitmap(i-__w, j, _buff+XY(i, j, __w), __w, 1); 
        matrix->drawRGBBitmap(__w - i, __h - j, _buff+XY(i, __h - j, __w), __w, 1); 
        matrix->show();    
        delay(10);
    //    matrix->drawPixel(i, j, matrix->Color(0,0,0));
    //    matrix->show();    
    //    delay(10);
      }
    }
    flip = false;
  }else {
    for (int16_t x = 0; x < __w; x++) {  
        for (int16_t y = 0; y < __h; y++) {
          uint32_t* p = (uint32_t*)_buff+XY(x, y, __w);
          uint32_t color = *p;
          uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
          uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
          uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
          matrix->drawPixel(x, y, matrix->Color(r,g,b) );
          matrix->show();    
          delay(10);
      }
    }
    flip = true;    
  }*/
    
}


  
  

  // How much time did rendering take
//  t = millis() - t;

//  char buf[80];
//  sprintf(buf, "%s %dx%d 1:%d %u ms", name, w, h, scale, t);
//  tft.setCursor(0, MX_HEIGHT - 8);
//  tft.print(buf);
//  Serial.println(buf);
//  delay(2000);
//}

void doOnJpgFileFoundCallbackWrapper(const char * file){
  matrix->clear();
  drawJpeg(file, 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image
  delay(__SPEED__);
  matrix->show();
}

void setupJPG(){
  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(false);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);  
}

#endif
