#ifndef _MEDIAHELPER_H_
#define _MEDIAHELPER_H_

#include "GifPlayer.h"
#include "jpgHelper.h"

GifPlayer gifPlayer;
//doOnFileFoundCallback


//void doOnJpgFileFoundCallbackWrapper(const char * file){
//  matrix->clear();
//  renderJpegToBuffer(file, 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image
//  delay(__SPEED__);
//  matrix->show();
//}
//
//void doOnGifFileFoundCallbackWrapper(const char * file){
//  Serial.print("  FILE: ");
//  Serial.println(file);
//  String d = String(file); //String(dir) + (String(dir).endsWith("/") ? "" : "/") + String(file);  
//  Serial.printf("Str file: %s", d);
//  File f = SPIFFS.open(d, "r");
//  playGif(&f);
//}



void playFile(File filename){  
  String _n = String (filename.name());
  if ( _n.endsWith(".gif") ){     
      gifPlayer.playGif(&filename);
      FastLED.show();
      delay(__SPEED__);
    };
  if ( _n.endsWith(".jpg") ){
    matrix->clear();
    renderJpegToBuffer(filename, 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image      );//String(__JPGS_FOLDER + "/" + filename).c_str(), 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image      
    renderBufferToMatrix( 0, 0, (uint16_t*)&drawTaskBitmapBuffer, 0);
    matrix->show();
    delay(__SPEED__);      
  }
}

void playMediaFile(String filename){  
    File _f;
    if ( filename.endsWith(".gif") ){
      _f = SPIFFS.open(__GIFS_FOLDER + "/" + filename);
      if ( _f ) {
        gifPlayer.playGif(&_f);
//        FastLED.show();
//        delay(__SPEED__);
      }
    };
    if ( filename.endsWith(".jpg") ){
      _f = SPIFFS.open(__JPGS_FOLDER + "/" + filename);      
      if ( _f ) {
      matrix->clear();
      renderJpegToBuffer(String(__JPGS_FOLDER + "/" + filename).c_str(), 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image      
      renderBufferToMatrix( 0, 0, (uint16_t*)&drawTaskBitmapBuffer, 0);
      matrix->show();
      delay(__SPEED__);      
    }
}
}

void playMedia(){
  if ( __MEDIA_FILE_NAME != "" ){
    playMediaFile(__MEDIA_FILE_NAME);
  }
}


void setupMedia(){

    setupJPG();
}

void doOnMediaFileFoundCallbackWrapper(File file){
  matrix->clear();
  playFile(file);//, 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image
  delay(__SPEED__);
  matrix->show();
}

#endif
