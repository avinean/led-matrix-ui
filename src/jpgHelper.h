#ifndef _JPGHELPER_H_
#define _JPGHELPER_H_

// JPEG decoder library
#include <TJpg_Decoder.h>

#include "soc/rtc_wdt.h"

#include "globals.h"
#include "matrixHelper.h"


bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);


bool flip = true;



void renderJpegToBuffer(File filename, int xpos, int ypos, uint16_t* _buff) {
  // Get the width and height in pixels of the jpeg if you wish  
  TJpgDec.getFsJpgSize(&__w, &__h, filename); // Note name preceded with "/"
//  tft.setRotation(w > h ? 1 : 0);
#if DEBUG == 1
Serial.print("Width = "); Serial.print(__w); Serial.print(", height = "); Serial.println(__h);
#endif
  for (__scale = 1; __scale <= 8; __scale <<= 1) {
    if (__w <= MX_WIDTH * __scale && __h <= MX_HEIGHT * __scale) break;
  }
  TJpgDec.setJpgScale(__scale);
  // Draw the image, top left at 0,0
  TJpgDec.drawFsJpg(0, 0, filename);    
}


void renderJpegToBuffer(const char *filename, int xpos, int ypos, uint16_t* _buff) {
  // Get the width and height in pixels of the jpeg if you wish  
  TJpgDec.getFsJpgSize(&__w, &__h, filename); // Note name preceded with "/"
//  tft.setRotation(w > h ? 1 : 0);
#if DEBUG == 1
Serial.print("Width = "); Serial.print(__w); Serial.print(", height = "); Serial.println(__h);
#endif
  for (__scale = 1; __scale <= 8; __scale <<= 1) {
    if (__w <= MX_WIDTH * __scale && __h <= MX_HEIGHT * __scale) break;
  }
  TJpgDec.setJpgScale(__scale);
  // Draw the image, top left at 0,0
  TJpgDec.drawFsJpg(0, 0, filename);    
}


void setupJPG(){
  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);
  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(false);
  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);  
}

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

#endif
