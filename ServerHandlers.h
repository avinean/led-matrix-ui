#ifndef _SERVERHANDLERS_H_
#define _SERVERHANDLERS_H_

// Import required libraries
#include "ESPAsyncWebServer.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

//#include "DefaultEndpoints.h"
//#include "MatrixHelper.h"

#include "globals.h"

#include "TasksHelper.h"
#include "EffectsHelper.h"



//unsigned char resp[NUM_LEDS];
DeserializationError err;


void enableGameMode(){
//    gamemodeFlag = true;
//    gamePaused = false;
//    gameDemo = false;
//    loadingFlag = true;
//    gameTimer.setInterval(DEMO_GAME_SPEED);
//    gameTimer.pause(false);
}

void disableGameMode(){
//    gamemodeFlag = false;
//    gamePaused = true;
//    gameDemo = false;
//    loadingFlag = false;
//    gameTimer.setInterval(0);
//    gameTimer.pause(true);
}

void disableCanvasMode(){  
//  loadingFlag = false;
}

void disableTextMode(){
//  loadingFlag = false;
//  scrollTimer.setInterval(0);
//  scrollTimer.pause(true);
}

void enableTextMode(){
//  loadingFlag = true;
//  scrollTimer.setInterval(_RUN_TEXT_SPEED_);
//  scrollTimer.pause(false);
}

void indexrequest(AsyncWebServerRequest *request){
  request->send_P(200, "text/html", upload_page);
}

static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.printf("Update: %s[%d]: dta: %2X, len=%d, final=%d...\n", filename.c_str(), index, *data, len, final );  
  
  if (!index){
    Serial.printf("!index\n"); 
    request->_tempFile = SPIFFS.open("/"+filename, FILE_WRITE);
    if (!request->_tempFile) {
      Serial.println("There was an error opening the file for writing");
      return;
    }
  }

  if ( request->_tempFile && len ){
    Serial.printf("request->_tempFile && len\n"); 
    request->_tempFile.write(data,len);
    Serial.println("..done");
  }

  if(final){
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    request->_tempFile.close();
    spiffsListDir();
  }
}

void setupServer(AsyncWebServer* server){
  // I guess this string can work the same way as setupDefaultEndpoints(&server)
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

server->on("/picture-effects", HTTP_GET, [](AsyncWebServerRequest *request){
  Serial.printf("/picture-effects(GET)\n");
  // Handling function
    StaticJsonDocument<400> data;
//    JsonObject root = data.to<JsonObject>();

    JsonArray arr = data.to<JsonArray>();//root.createNestedArray("effects");
    arr.add("MirroredNoise");
    arr.add("RedClouds");
    arr.add("Lavalamp1");
    arr.add("Lavalamp2");
    arr.add("Lavalamp3");
    arr.add("Lavalamp4");
    arr.add("Lavalamp5");
    arr.add("Constrained1");
    arr.add("RelativeMotion1");
    arr.add("Water");
    arr.add("Bubbles1");
    arr.add("TripleMotion");
    
//    data["width"] = MX_WIDTH;
//    data["height"] = MX_HEIGHT;
    
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
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
        _EFFECT_SPEED = obj["speed"];
        
        Serial.printf("Effect ID: %u, speed: %u\n\n", _EFFECT_ID, _EFFECT_SPEED);        

        startTask(
          effectsTaskCode,   /* Task function. */
          "effects",     /* name of task. */
          2000 ,       /* Stack size of task */
          NULL,        /* parameter of the task */
          2,           /* priority of the task */          
          1);          /* pin task to core 0 */   
        
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

  server->on("/updatefw", HTTP_GET, indexrequest);

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
      disableGameMode();
      disableCanvasMode();
      disableTextMode();        
      __CURRENT_MODE = __MODE_GAME;
      enableGameMode();        
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
    AsyncWebServerResponse *response = request->beginChunkedResponse("application/octet-stream", 
      [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
          Serial.printf("\n/matrix-state[%u]:%3d, %3d\n", NUM_LEDS, index, maxLen);
          if( index){ //already sent
              return 0;
          }
          int res = (maxLen > BITMAP_SIZE) ? BITMAP_SIZE : maxLen;
          memmove( buffer, &leds, res );
          return res;
      });
    response->addHeader("Cache-Control", "no-cache");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server->on("/matrix-parameters", HTTP_GET, [](AsyncWebServerRequest *request){
  // Handling function
    StaticJsonDocument<500> data;
    JsonObject root = data.to<JsonObject>();
    
    root["width"] = MX_WIDTH;
    root["height"] = MX_HEIGHT;

  
    JsonArray arr = root.createNestedArray("effects");
    arr.add("MirroredNoise");
    arr.add("RedClouds");
    arr.add("Lavalamp1");
    arr.add("Lavalamp2");
    arr.add("Lavalamp3");
    arr.add("Lavalamp4");
    arr.add("Lavalamp5");
    arr.add("Constrained1");
    arr.add("RelativeMotion1");
    arr.add("Water");
    arr.add("Bubbles1");
    arr.add("TripleMotion");
    
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
        __CURRENT_MODE = __MODE_TEXT; 
        disableGameMode();
        disableCanvasMode();
        disableTextMode();        
        if (obj.containsKey("speed")) {
          _RUN_TEXT_SPEED_ = obj["speed"].as<unsigned char>();
        }
        unsigned char r = obj["color"]["r"];
        unsigned char g = obj["color"]["g"];
        unsigned char b = obj["color"]["b"];
        __RUNNING_STRING_COLOR = CRGB( r, g, b );
        unsigned char br = obj["backgroundColor"]["r"];
        unsigned char bg = obj["backgroundColor"]["g"];
        unsigned char bb = obj["backgroundColor"]["b"];
        __RUNNING_STRING_BACKGROUND_COLOR = CRGB( br, bg, bb );
        enableTextMode();
        Serial.printf("str = %s, spd = %u, r: %u g: %u b: %u, br: %u bg: %u bb: %u\n\n", (String)_RUN_TEXT_.c_str(), (int)_RUN_TEXT_SPEED_, r, g, b, br, bg, bb); 

startTask(
    scrollTextTaskCode,   /* Task function. */
    "scrollText",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    2,           /* priority of the task */    
    1);
 
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

//startDrawTask();
drawTaskBitmapInfo = (bitmapInfo){ (const short unsigned int*)&drawTaskBitmapBuffer, 16, 16, 0 };
startTask(
    drawTaskCode,   /* Task function. */
    "scrollText",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    2,           /* priority of the task */    
    1);          /* pin task to core 0 */                  

    disableGameMode();
    disableCanvasMode();
    disableTextMode();        
    __CURRENT_MODE = __MODE_CANVAS;    
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
      disableGameMode();
      disableCanvasMode();
      disableTextMode();        
      __CURRENT_MODE = __MODE_CANVAS;

      matrix->drawPixel(x, y, CRGB( r, g, b ));
      matrix->show();
      request->send(200, "text/plain", "ack");
    } else
      request->send(404, "text/plain", ":(");
  });

  // Start server
  server->begin();  
}

#endif //_SERVERHELPER_H_