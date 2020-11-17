#ifndef _JPEG_functions_H_
#define _JPEG_functions_H_

//#define DEBUG 1

/*====================================================================================
  This sketch contains support functions to render the Jpeg images.

  Created by Bodmer 15th Jan 2017
  Updated by Bodmer to support ESP32 with SPIFFS Jan 2018
  ==================================================================================*/

void jpegInfo();
void createArray(const char *filename);
void jpegRender(int xpos, int ypos, uint16_t *buff);

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
void drawJpeg(const char *filename, int xpos, int ypos, uint16_t *buff) {

#if DEBUG == 1
  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");
#endif  

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  //ESP32 always seems to return 1 for jpegFile so this null trap does not work
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder,
  // the filename can be a String or character array type:

  //boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)

  if (decoded) {
#if DEBUG == 1    
    // print information about the image to the serial port
    jpegInfo();
#endif    

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos, buff);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}

//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void jpegRender(int xpos, int ypos, uint16_t *buff) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  int16_t mcu_w = JpegDec.MCUWidth;
  int16_t mcu_h = JpegDec.MCUHeight;
  int32_t max_x = JpegDec.width;
  int32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  int32_t min_w = minimum(mcu_w, max_x % mcu_w);
  int32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  int32_t win_w = mcu_w;
  int32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
//  while ( JpegDec.readSwappedBytes()) { // Swapped byte order read
  while ( JpegDec.read()) { // Swapped byte order read

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) 
      win_w = mcu_w;
    else 
      win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) 
      win_h = mcu_h;
    else 
      win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w){
      Serial.printf("win_w(%d) != mcu_w(%d), win_w << 1: %d\n", win_w, mcu_w, win_w << 1);
      for (int h = 1; h < win_h-1; h++){
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
//        if ( buff != NULL ) {
//          Serial.printf("\n__!!!!__: buff: %d, pImg: %d, win_w: %d, win_h: %d, sz: %d; mcu_w: %d, mcu_h: %d; mcu_x: %d, mcu_y: %d\n", buff, pImg, win_w, win_h, win_w * win_h, mcu_w, mcu_h, mcu_x, mcu_y);
//          memcpy(buff + h * MX_WIDTH, pImg + h * win_w, win_w << 1);
//        }
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( mcu_x < MX_WIDTH && mcu_y < MX_HEIGHT){
      if ( buff == NULL ) {
        Serial.printf("\n!!!!: pImg: %d, win_w: %d, win_h: %d, sz: %d; mcu_w: %d, mcu_h: %d; mcu_x: %d, mcu_y: %d\n", pImg, win_w, win_h, win_w * win_h, mcu_w, mcu_h, mcu_x, mcu_y);
//        dumpPtr((const uint8_t*)&pImg, mcu_w * mcu_h);
        // Now push the image block to the screen
        matrix->drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h); 
      } 
      else {
        Serial.printf("\n__!!!!__: buff: %d, pImg: %d, win_w: %d, win_h: %d, sz: %d; mcu_w: %d, mcu_h: %d; mcu_x: %d, mcu_y: %d\n", buff, pImg, win_w, win_h, win_w * win_h, mcu_w, mcu_h, mcu_x, mcu_y);
        int16_t y = mcu_y;
        int16_t x = mcu_x;
        uint16_t *bptr = pImg;
        memset (buff, 0, sizeof(buff));
        for (int16_t j = 0; j < win_h-1; j++, y++) {          
          memcpy(buff + XY( x, y, mcu_w/*(win_w > 1 ? win_w : MX_WIDTH )*/) - j * 2, pImg + j * win_w, win_w << 1);
          Serial.printf("x: %d, y: %d, j: %d, XY(): %d, pImg: %d\n", x, y, j, buff + XY( x, y, mcu_w), pImg + j * win_w);
//          for (int16_t i = 0; i < win_w; i++) {
////            buff[j * win_w + i] = pImg[j * win_w + i];
//              buff[XY( x + i, y)] = bptr[j * win_w + i];
////            buff[j * MX_WIDTH + i] = pImg[j * win_w + i];
//          }
        }
//        dumpPtr((const uint8_t*)&buff, MX_WIDTH * MX_HEIGHT);     
//        for (int h = 1; h < win_h-1; h++){
//          memcpy(buff + (h-1) * win_w, pImg + (h - 1) * win_w , win_w);
//        }
//        for (int h = 0; h < win_h; h++){
//            memcpy(buff + h * win_w, pImg + h * mcu_w , win_w << 1);
//          }
      }
    } else 
    if ( ( mcu_y + win_h) >= MX_HEIGHT) {
      JpegDec.abort();
    }
  }

  drawTaskBitmapInfo = (bitmapInfo){ (const short unsigned int*)buff, JpegDec.width, JpegDec.height, 0 };


#if DEBUG == 1
  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took
  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");
#endif
}

//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void jpegInfo() {

  Serial.println("===============");
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print  ("Width      :"); Serial.println(JpegDec.width);
  Serial.print  ("Height     :"); Serial.println(JpegDec.height);
  Serial.print  ("Components :"); Serial.println(JpegDec.comps);
  Serial.print  ("MCU / row  :"); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  ("MCU / col  :"); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  ("Scan type  :"); Serial.println(JpegDec.scanType);
  Serial.print  ("MCU width  :"); Serial.println(JpegDec.MCUWidth);
  Serial.print  ("MCU height :"); Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}

#if DEBUG == 1
//====================================================================================
//   Open a Jpeg file and send it to the Serial port in a C array compatible format
//====================================================================================
void createArray(const char *filename) {
  

  // Open the named file
  fs::File jpgFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpgFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library

  if ( !jpgFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  uint8_t data;
  byte line_len = 0;
  Serial.println("");
  Serial.println("// Generated by a JPEGDecoder library example sketch:");
  Serial.println("// https://github.com/Bodmer/JPEGDecoder");
  Serial.println("");
  Serial.println("#if defined(__AVR__)");
  Serial.println("  #include <avr/pgmspace.h>");
  Serial.println("#endif");
  Serial.println("");
  Serial.print  ("const uint8_t ");
  while (*filename != '.') Serial.print(*filename++);
  Serial.println("[] PROGMEM = {"); // PROGMEM added for AVR processors, it is ignored by Due

  while ( jpgFile.available()) {

    data = jpgFile.read();
    Serial.print("0x"); if (abs(data) < 16) Serial.print("0");
    Serial.print(data, HEX); Serial.print(",");// Add value and comma
    line_len++;
    if ( line_len >= 32) {
      line_len = 0;
      Serial.println();
    }

  }

  Serial.println("};\r\n");
  jpgFile.close();
}

#endif
//====================================================================================

#endif
