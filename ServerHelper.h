#ifndef _SERVERHELPER_H_
#define _SERVERHELPER_H_

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

//#include "DefaultEndpoints.h"
#include "MatrixHelper.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned char resp[BITMAP_SIZE];
DeserializationError err;


void enableGameMode(){
    gamemodeFlag = true;
    gamePaused = false;
    gameDemo = false;
    loadingFlag = true;
    gameTimer.setInterval(DEMO_GAME_SPEED);
    gameTimer.pause(false);
}

void disableGameMode(){
    gamemodeFlag = false;
    gamePaused = true;
    gameDemo = false;
    loadingFlag = false;
    gameTimer.setInterval(0);
    gameTimer.pause(true);
}

void disableCanvasMode(){  
  loadingFlag = false;
}

void disableTextMode(){
  loadingFlag = false;
  scrollTimer.setInterval(0);
  scrollTimer.pause(true);
}

void enableTextMode(){
  loadingFlag = true;
  scrollTimer.setInterval(_runningStringSpeed);
  scrollTimer.pause(false);
}

void setupServer(){
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // I guess this string can work the same way as setupDefaultEndpoints(&server)
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
// Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
  server.on("/control/up", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/control/up");
    buttons = 0;
  });

  server.on("/control/right", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/control/right");
    buttons = 1;
  });

  server.on("/control/down", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/control/down");
    buttons = 2;
  });

  server.on("/control/left", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/control/left");
    buttons = 3;
  });

  server.on("/games", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/games");
    DynamicJsonDocument root(128);
    err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();  
        _GAME_NAME_ = obj["game"].as<String>();
        Serial.printf("%s\n\n", _GAME_NAME_.c_str());
        disableGameMode();
        disableCanvasMode();
        disableTextMode();        
        __CURRENT_MODE = __MODE_GAME;
        enableGameMode();        
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
  });

  server.on("/get_pixel", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/matrix-state[%u]", BITMAP_SIZE);
  });

  server.on("/fill-matrix", HTTP_POST, [](AsyncWebServerRequest *request){
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
        FastLED.clear();
        fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
  });

  server.on("/matrix-state", HTTP_GET, [](AsyncWebServerRequest *request){  
    AsyncWebServerResponse *response = request->beginChunkedResponse("application/octet-stream", 
      [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
          Serial.printf("\n/matrix-state[%u]:%3d, %3d\n", BITMAP_SIZE, index, maxLen);
          if( __CURRENT_MODE == __MODE_GAME || index){ //already sent
              return 0;
          }          
          getCanvasPixels(resp);
          int res = BITMAP_SIZE;
          memmove( buffer, &resp, BITMAP_SIZE );
          return res;
      });
    response->addHeader("Cache-Control", "no-cache");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/matrix-parameters", HTTP_GET, [](AsyncWebServerRequest *request){
  // Handling function
    StaticJsonDocument<100> data;
    data["width"] = MX_WIDTH;
    data["height"] = MX_HEIGHT;
    
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });


  server.on("/running-text", HTTP_POST, [](AsyncWebServerRequest *request){
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      Serial.printf("/running-text:\n");
      DynamicJsonDocument root(255);
      err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();
        if (obj.containsKey("string")) {
          _runningString = obj["string"].as<String>();
        }
        __CURRENT_MODE = __MODE_TEXT; 
        disableGameMode();
        disableCanvasMode();
        disableTextMode();        
        if (obj.containsKey("speed")) {
          _runningStringSpeed = obj["speed"].as<unsigned char>();
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
        Serial.printf("str = %s, spd = %u, r: %u g: %u b: %u, br: %u bg: %u bb: %u\n\n", _runningString.c_str(), _runningStringSpeed, r, g, b, br, bg, bb); 
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
    });

server.on("/draw", HTTP_POST, [](AsyncWebServerRequest *request){
    //nothing and dont remove it
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/draw: len=%u, idx=%u, ttl=%u\n", len, index, total);
    memmove( &bitmap, data, BITMAP_SIZE );
//    int idx = 0;
//    for ( int y = 0; y < MX_HEIGHT; y++ ){
//      for ( int x = 0; x < MX_WIDTH; x++ ){
//        idx = (x + y * MX_WIDTH) * 3;
//          if ( ( data[idx] >= BG_COLOR_THRESH_MAX  ) && ( data[idx+1]  >= BG_COLOR_THRESH_MAX  ) && ( data[idx+2]  >= BG_COLOR_THRESH_MAX ) ){
//            bitmap[idx] = 0x00;
//            bitmap[idx+1] = 0x00;
//            bitmap[idx+2] = 0x00;            
//          } else {
//            bitmap[idx] = data[idx];
//            bitmap[idx+1] = data[idx+1];
//            bitmap[idx+2] = data[idx+2];            
//          }
//          Serial.printf("%3u,%3u,%3u  ", data[idx], data[idx + 1], data[idx + 2]);
//      }
//      Serial.println();
//    }
    disableGameMode();
    disableCanvasMode();
    disableTextMode();        
    __CURRENT_MODE = __MODE_CANVAS;    
//    fillCanvas(data);
    request->send(200, "text/plain", "ack");
  });

server.on("/pixel", HTTP_POST, [](AsyncWebServerRequest *request){
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
//      enableCanvasMode();
      int idx = (x + y * MX_WIDTH) * DEPTH;
      bitmap[idx] = r;
      bitmap[idx+1] = g;
      bitmap[idx+2] = b;
      request->send(200, "text/plain", "ack");
    } else
      request->send(404, "text/plain", ":(");
  });

  // Start server
  server.begin();  
}

#endif //_SERVERHELPER_H_
