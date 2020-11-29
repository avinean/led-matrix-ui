#ifndef _MODESHELPER_H_
#define _MODESHELPER_H_

#include "mediaHelper.h"

#include "gamesHelper.h"
#include "GfxHelper.h"
#include "EffectsHelper.h"
//#include "jpgHelper.h"




void __FNC_MODE_JPG(){
//  String path = __JPGS_FOLDER + "/" + __imgName;
//  doOnJpgFileFoundCallbackWrapper(path.c_str());
playMediaFile(__imgName);
}

void __FNC_MODE_GIF(){
//      String path = __GIFS_FOLDER + "/" + __imgName;
//      doOnGifFileFoundCallbackWrapper(path.c_str());
      playMediaFile(__imgName);
    };
    
void __FNC_MODE_BOOT(){
  display_scrollText_multyline(__BOOT_STR__, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
  FastLED.show();
//  break;
};
void __FNC_MODE_DRAW(){      
//  break;
};
void __FNC_MODE_GAME(){
  gameRoutine();
//  break;
};
void __FNC_MODE_CANVAS(){
  display_panOrBounceBitmap(&drawTaskBitmapInfo);
  FastLED.show();
//  break;
};
void __FNC_MODE_TEXT(){      
  display_scrollText(_RUN_TEXT_, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
  FastLED.show();
//  break;
};
void __FNC_MODE_EFFECTS(){      
  RunAnimationDependingOnPgm();
//      FastLED.show();
//  break;
};
void __FNC_MODE_GIF_PLAY(){      
  findFilesInDir(SPIFFS, __GIFS_FOLDER.c_str(), ".gif", 3, doOnMediaFileFoundCallbackWrapper);     
  FastLED.show();
//  break;
};
void __FNC_MODE_JPG_PLAY(){      
  findFilesInDir(SPIFFS, __JPGS_FOLDER.c_str(), ".jpg", 3, doOnMediaFileFoundCallbackWrapper);
  FastLED.show();
//  break;
};   


    
#define __MODES_COUNT_ 9
funcPtrInfo __MODES_FNCS_[] = {
    { "BOOT", __MODE_BOOT,    &__FNC_MODE_BOOT },
    { "CANVAS", __MODE_CANVAS,  &__FNC_MODE_CANVAS },
    { "TEXT", __MODE_TEXT,    &__FNC_MODE_TEXT },
    { "GAME", __MODE_GAME,    &__FNC_MODE_GAME },
    { "GIF", __MODE_GIF,     &__FNC_MODE_GIF },
    { "GIF_PLAY", __MODE_GIF_PLAY, &__FNC_MODE_GIF_PLAY },
    { "JPG", __MODE_JPG,     &__FNC_MODE_JPG },
    { "JPG_PLAY", __MODE_JPG_PLAY,&__FNC_MODE_JPG_PLAY },
    { "EFFECTS", __MODE_EFFECTS, &__FNC_MODE_EFFECTS }
};
//funcPtrInfo* __CURRENT_MODE_FUNC;//=NULL;
//funcPtrInfo* __LAST_MODE_FUNC;


#endif
