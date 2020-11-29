#ifndef _SERVERHANDLERS_H_
#define _SERVERHANDLERS_H_

// Import required libraries
#include "ESPAsyncWebServer.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

#include "globals.h"

//#include "EffectsHelper.h"
#include "SPIFFS_functions.h"

//#include "jpgHelper.h"
#include "EffectsHelper.h"

#include "modesHelper.h"

//unsigned char resp[NUM_LEDS];
DeserializationError err;
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
//    client->text("Hello from ESP32 Server");
    globalClient = client;
    globalClient->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
      globalClient= NULL;
  }
}


void enableGameMode(){
    gamemodeFlag = true;
    gamePaused = false;
//    gameDemo = false;
    loadingFlag = true;
    gameTimer.setInterval(__SPEED__);
    gameTimer.pause(false);
}

void disableGameMode(){
    gamemodeFlag = false;
    gamePaused = true;
//    gameDemo = false;
    loadingFlag = false;
    gameTimer.setInterval(0);
    gameTimer.pause(true);
}


void _disableAll(){
  disableGameMode();  
}

const uint8_t __files_max = 50;
String __files[__files_max];
uint8_t __files_idx = 0;
uint8_t __files_cnt = 0;

void clearFilesList(){
    for ( int i = 0; i  < __files_max; i++){
        __files[i] = "";
    }
    __files_idx = 0;
    __files_cnt = 0;
}

void collectFiles(File file){
    __files[__files_idx++] = file.name();
    __files_cnt++;
}

void findImages(String fileType) {
    clearFilesList();
    if ( fileType ==".gif" ){
        findFilesInDir(SPIFFS, __GIFS_FOLDER.c_str(), fileType.c_str(), 3, collectFiles);
    } else
    if ( fileType == ".jpg" ){
        findFilesInDir(SPIFFS, __JPGS_FOLDER.c_str(), fileType.c_str(), 3, collectFiles);
    }
}



void setupServer(AsyncWebServer* server){
  // I guess this string can work the same way as setupDefaultEndpoints(&server)
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server->on("/show", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/show\n");
    _disableAll();
    //Check if parameter exists (Compatibility)
    if(request->hasParam("img")){
      _disableAll();
      __imgName = request->getParam("img")->value();
      String path = "/";
      if ( __imgName.endsWith(".gif") ){
        __NEXT_MODE = __MODE_GIF;
      } else
      if ( __imgName.endsWith(".jpg") ){
        __NEXT_MODE = __MODE_JPG;        
      }
    }
      __MODE_STOPED_FLAG = false;
    request->send(200, "text/plain", "ack");
  });

  server->on("/play_gifs", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/play_gifs\n");
    _disableAll();
    //Check if parameter exists (Compatibility)
    if(request->hasArg("speed"))
      __SPEED__ = request->arg("speed").toInt();    
    __NEXT_MODE = __MODE_GIF_PLAY;
    request->send(200, "text/plain", "ack");
  });

  server->on("/play_jpgs", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/play_jpgs\n");
    _disableAll();
    //Check if parameter exists (Compatibility)
    if(request->hasArg("speed"))
      __SPEED__ = request->arg("speed").toInt();
    __NEXT_MODE = __MODE_JPG_PLAY;
    request->send(200, "text/plain", "ack");
  });  


  server->on("/picture-effects", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      Serial.printf("/picture-effects(POST)\n");
      DynamicJsonDocument root(128);
      err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();  
        _EFFECT_ID = obj["effect"];
        __SPEED__ = obj["speed"];
        
        Serial.printf("Effect ID: %u, speed: %u\n\n", _EFFECT_ID, __SPEED__);        

        _disableAll();       
        __NEXT_MODE = __MODE_EFFECTS;        
        new_pattern = 1;
//        item = 
        
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
  });

  server->on("/rotate", HTTP_POST, [](AsyncWebServerRequest *request){    
    _MX_ROTATION =  ( ++_MX_ROTATION > 3 ? 0 : _MX_ROTATION );
    Serial.printf("/rotate: %d\n", _MX_ROTATION);
    matrix->setRotation(_MX_ROTATION);    
    request->send(200, "text/plain", "ack");
  });

  // handler for the /update form POST (once file upload finishes)
  server->on("/send-file", HTTP_POST, [](AsyncWebServerRequest *request){    
    request->send(200);
  }, handle_update_progress_cb);

  server->onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
  
  
// Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
  server->on("/up", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.printf("/up\n");
    _GAME_BUTTONS_ = 0;
    request->send(200, "text/plain", "ack");
  });

  server->on("/right", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.printf("/right\n");
    _GAME_BUTTONS_ = 1;
    request->send(200, "text/plain", "ack");
  });

  server->on("/down", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.printf("/down\n");
    _GAME_BUTTONS_ = 2;
    request->send(200, "text/plain", "ack");
  });

  server->on("/left", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.printf("/left\n");
    _GAME_BUTTONS_ = 3;
    request->send(200, "text/plain", "ack");
  });

  server->on("/games", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/games");
    DynamicJsonDocument root(128);
    err = deserializeJson(root, (const char*)data);
    if (err == DeserializationError::Ok) {
      // Get a reference to the root object
      JsonObject obj = root.as<JsonObject>();  
      _GAME_NAME_ = obj["game"].as<String>();
      Serial.printf("%s\n\n", (const char**)(_GAME_NAME_.c_str()));
      _disableAll();
      __NEXT_MODE = __MODE_GAME;
//      __CURRENT_MODE_FUNC = &__FNC_MODE_GAME;
//      enableGameMode();
//      __MODE_STOPED_FLAG = false;
      request->send(200, "text/plain", "ack");
    } else
      request->send(404, "text/plain", ":(");
  });

  server->on("/fill-matrix", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      Serial.printf("/fill-matrix:\n");
      DynamicJsonDocument root(128);
      err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();  
        unsigned char r = obj["r"];
        unsigned char g = obj["g"];
        unsigned char b = obj["b"];  
        Serial.printf("r: %u g: %u b: %u\n\n", r, g, b);        
        
        matrix->clear();
        matrix->fillScreen(CRGB( r, g, b ));
        matrix->show();
        
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
  });

  server->on("/matrix-state", HTTP_GET, [](AsyncWebServerRequest *request){  
//    AsyncWebServerResponse *response = request->beginChunkedResponse("application/octet-stream", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
      AsyncWebServerResponse *response = request->beginResponse("application/octet-stream", BITMAP_SIZE, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
#if DEBUG == 1
      Serial.printf("\n/matrix-state[%u]:%3d, %3d\n", NUM_LEDS, index, maxLen);
#endif
//      if( index){ //already sent
//          return 0;
//      }
//      int res = (maxLen > BITMAP_SIZE) ? BITMAP_SIZE : maxLen;
      memmove( buffer, &matrixleds, BITMAP_SIZE );
      return BITMAP_SIZE;
    });
    response->addHeader("Cache-Control", "no-cache");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

    server->on("/animations", HTTP_GET, [](AsyncWebServerRequest *request) {
// Handling function
        StaticJsonDocument<500> data;
        JsonObject root = data.to<JsonObject>();

        findImages(".gif");

        JsonArray arr = root.createNestedArray("gifsList");
        for ( int i = 0; i <  __files_cnt; i++ ){
            Serial.println(__files[i]);
            arr.add(__files[i]);
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
    });

    server->on("/images", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Handling function
        StaticJsonDocument<500> data;
        JsonObject root = data.to<JsonObject>();

        findImages( ".jpg");

        JsonArray arr = root.createNestedArray("imageList");
        for ( int i = 0; i <  __files_cnt; i++ ){
            Serial.println(__files[i]);
            arr.add(__files[i]);
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
    });

  server->on("/matrix-parameters", HTTP_GET, [](AsyncWebServerRequest *request){
  // Handling function
    StaticJsonDocument<500> data;
    JsonObject root = data.to<JsonObject>();
    
    root["width"] = MX_WIDTH;
    root["height"] = MX_HEIGHT;

    root["speed"] = __SPEED__;
    root["speedMax"] = __SPEED_MAX__;
    root["brightness"] = FastLED.getBrightness();
    
      
    JsonArray arr = root.createNestedArray("effects");
    for ( int i = 0; i <  effectsCount; i++ ){
      Serial.println(items[i]->name);
      arr.add(items[i]->name);
    }
    arr = root.createNestedArray("games");
    for ( int i = 0; i < gamesCount; i++ ) {
      arr.add(__GAMES__[i].name);
    }   
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  server->on("/running-text", HTTP_POST, [](AsyncWebServerRequest *request){
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/running-text:\n");
    
    DynamicJsonDocument root(512);
    err = deserializeJson(root, (const char*)data);
    if (err == DeserializationError::Ok) {
      // Get a reference to the root object
      JsonObject obj = root.as<JsonObject>();
      if (obj.containsKey("string")) {
        _RUN_TEXT_ = obj["string"].as<String>();
      }      
      _disableAll();
      if (obj.containsKey("speed")) {
        __SPEED__ = obj["speed"].as<unsigned char>();
      }
      unsigned char r = obj["color"]["r"];
      unsigned char g = obj["color"]["g"];
      unsigned char b = obj["color"]["b"];
      __RUNNING_STRING_COLOR = CRGB( r, g, b );
      unsigned char br = obj["backgroundColor"]["r"];
      unsigned char bg = obj["backgroundColor"]["g"];
      unsigned char bb = obj["backgroundColor"]["b"];
      __RUNNING_STRING_BACKGROUND_COLOR = CRGB( br, bg, bb );
      
      Serial.printf("str = %s, spd = %u, r: %u g: %u b: %u, br: %u bg: %u bb: %u\n\n", _RUN_TEXT_.c_str(), (int)__SPEED__, r, g, b, br, bg, bb);
      __NEXT_MODE = __MODE_TEXT; 
      request->send(200, "text/plain", "ack");
    } else
      request->send(404, "text/plain", ":(");
  });

  server->on("/draw", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/draw: len=%u, idx=%u, ttl=%u\n", len, index, total);
  
    uint16_t _didx = 0;
    for ( uint16_t _idx = 0; _idx < NUM_LEDS; _idx++ ){
      drawTaskBitmapBuffer[_idx] = matrix->Color(data[_didx++], data[_didx++], data[_didx++]);
    }
    drawTaskBitmapInfo = (bitmapInfo){ (const short unsigned int*)&drawTaskBitmapBuffer, 16, 16, 0 };
  
    _disableAll();    
    __NEXT_MODE = __MODE_CANVAS;    
    request->send(200, "text/plain", "ack");
  });

  server->on("/pixel", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/pixel:\n");
    DynamicJsonDocument root(128);
    err = deserializeJson(root, (const char*)data);
    if (err == DeserializationError::Ok) {
      // Get a reference to the root object
      JsonObject obj = root.as<JsonObject>();
      unsigned char x = obj["x"];
      unsigned char y = obj["y"];
      unsigned char r = obj["r"];
      unsigned char g = obj["g"];
      unsigned char b = obj["b"];
      Serial.printf("(%u:%u) r: %u g: %u b: %u\n\n", x, y, r, g, b);        
      if  ( __CURRENT_MODE == __MODE_CANVAS ){
        if ( ! MODE_DRAW_FLAG )
          MODE_DRAW_FLAG = true;
        matrix->drawPixel(x, y, CRGB( r, g, b ));
        matrix->show();
      }     
      request->send(200, "text/plain", "ack");
    } else
      request->send(404, "text/plain", ":(");
  });


  ws.onEvent(onWsEvent);
  server->addHandler(&ws);
  // Start server
  server->begin();  
}


bool __UPLOADING_MEDIA_FLAG = false;
bool __UPLOADING_MEDIA_SKIP = false;
uint8_t __MEDIA_MODE = __MODE_NULL;
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.printf("Update: %s[%d]: dta: %2X, len=%d, final=%d...\n", filename.c_str(), index, *data, len, final );  
  
  if (!index){    
    String path = "/";
    if ( filename.endsWith(".gif") ){
      __UPLOADING_MEDIA_FLAG = true;
      __MEDIA_MODE = __MODE_GIF;
      path = __GIFS_FOLDER + "/";
    } else
    if ( filename.endsWith(".jpg") ){
      __UPLOADING_MEDIA_FLAG = true;
      __MEDIA_MODE = __MODE_JPG;
      path = __JPGS_FOLDER + "/";
    } else
      __UPLOADING_MEDIA_FLAG = false;
    if ( !SPIFFS.exists(String(path + filename).c_str()) ) {
        Serial.printf("!index path = %s\n", path.c_str());
        request->_tempFile = SPIFFS.open(path + filename, FILE_WRITE);
        if (!request->_tempFile) {
            Serial.println("There was an error opening the file for writing");
            return;
        }
    } else {
        __UPLOADING_MEDIA_SKIP = true;
    }
  }

  

  if ( !__UPLOADING_MEDIA_SKIP && request->_tempFile && len ){
    Serial.printf("request->_tempFile && len\n"); 
    request->_tempFile.write(data,len);
    Serial.println("..done");
  }

  if(final){
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    if ( !__UPLOADING_MEDIA_SKIP ) {
        request->_tempFile.close();
    }
//    listFiles();
    if ( __UPLOADING_MEDIA_FLAG && __UPLOADING_MEDIA_SKIP ) {
      __UPLOADING_MEDIA_FLAG = false;
        __UPLOADING_MEDIA_SKIP = false;
        __imgName = filename;
      __NEXT_MODE = __MEDIA_MODE;
    }
  }
}

#endif //_SERVERHELPER_H_
