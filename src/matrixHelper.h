#ifndef _MATRIX_HAL_H_
#define _MATRIX_HAL_H_

uint16_t XY( uint8_t x, uint8_t y);
uint16_t XY( uint8_t x, uint8_t y, uint8_t w);

void renderBufferToMatrix(int xpos, int ypos, uint16_t* _buff, uint8_t mode) {
//  tft.fillScreen(TFT_RED);
//  matrix->clear();  
//  matrix->fillScreen(CRGB::Green);
//  matrix->show();

matrix->clear();
  for (int16_t j = 0; j < __h / 2; j++) {
    for (int16_t i = 0; i < __w; i++) {      
      matrix->drawRGBBitmap(i-__w, j, _buff+XY(i, j, __w), __w, 1); 
      matrix->drawRGBBitmap(__w - i, __h - j, _buff+XY(i, __h - j, __w), __w, 1); 
      matrix->show();    
      delay(100);
    }
  }
  

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

  

//     for (int16_t j = 0; j < __h; j++) {
//      for (int16_t i = 0; i < __w; i++) {      
//        matrix->drawRGBBitmap(i, j, _buff+XY(i, j, __w), __w, 1); 
//        rtc_wdt_feed();matrix->show();    
//        delay(5);
//      }
//    }

}


CRGB c565toCRGB( uint16_t color){
  uint8_t r = ((color >> 11) & 0x1F);
  uint8_t g = ((color >> 5) & 0x3F);
  uint8_t b = (color & 0x1F);
  return CRGB(r,g,b);
}

RGB565 CRGBto565(CRGB _color){
  uint32_t color = (uint32_t)_color;//gammaCorrection( leds[XY(x,y)] );
  RGB565 res;
  res.r  = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  res.g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6; 
  res.b = (((color & 0x1F) * 527) + 23) >> 6;
//  matrix->drawPixel(x, y, matrix->Color( r,g,b ) );
return res;
}


void dumpCRGB(CRGB color){
  Serial.printf("R:%2X, G:%2X, B:%2X", color.r, color.g, color.b);
}

CRGB getPixColorXYCRGB ( uint16_t x, uint16_t y ){
    return effects.leds[XY_EF(x, y)];//leds[ XY(x, y)];
}


// Translate the x/y coordinates into the right index in the
// framebuffer.
// The Smartmatrix has a simple line by line layout, no 
// serpentines. It safed 2 fps to keep this function short.
// The function is called (sometimes) over 200 000 times per second!

uint16_t XY( uint8_t x, uint8_t y) {
  uint16_t i;
  i = (y * MX_WIDTH) + x;
  return i;
}

uint16_t XY( uint8_t x, uint8_t y, uint8_t w) {
  uint16_t i;
  i = (y * w) + x;
  return i;
}


uint32_t getPixColorXY ( uint16_t x, uint16_t y ){
//  return (uint32_t)leds[ XY(x, y)];
  return matrix->XY(y,x);
}

void dumpPtr(const uint8_t* fdst, uint16_t fsize){
    Serial.println("=====================================");    
    for ( uint16_t i =0; i< fsize; i++ ){
      Serial.printf("%2X ", fdst[i]);            
    }
    Serial.println("=====================================");
}



#endif
