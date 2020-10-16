#ifndef _SERVERHELPER_H_
#define _SERVERHELPER_H_

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

#include "DefaultEndpoints.h"
#include "MatrixHelper.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned char resp[BITMAP_SIZE];

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
  server.serveStatic("/", SPIFFS, "/data/").setDefaultFile("index.html");
  // setupDefaultEndpoints(&server);

  server.on("/get_pixel", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("/matrix-state[%u]", BITMAP_SIZE);
  });


  server.on("/fill-matrix", HTTP_POST, [](AsyncWebServerRequest *request){
      //nothing and dont remove it
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      Serial.printf("/fill-matrix:\n");
      DynamicJsonDocument root(128);
  //      JsonObject& root = jsonBuffer.parseObject((const char*)data);
      DeserializationError err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();
  
        unsigned char r = obj["r"];//.as<char>();
        unsigned char g = obj["g"];//.as<char>();
        unsigned char b = obj["b"];//.as<char>();
  
        Serial.printf("r: %u g: %u b: %u\n\n", r, g, b);
          
  //      int idx = (x + y * MX_WIDTH) * DEPTH;
  //      bitmap[idx] = r;
  //      bitmap[idx+1] = g;
  //      bitmap[idx+2] = b;
  
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
  });


  server.on("/matrix-state", HTTP_GET, [](AsyncWebServerRequest *request){  
    AsyncWebServerResponse *response = request->beginChunkedResponse("application/octet-stream", 
      [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
          Serial.printf("\n/matrix-state[%u]:%3d, %3d\n", BITMAP_SIZE, index, maxLen);
          if(index){ //already sent
              return 0;
          }
          
          getCanvasPixels(resp);
  //        dumpCanvasPixels(resp);
//          Serial.printf("resp: %s\n", resp);
          int res = BITMAP_SIZE;//snprintf((char *)buffer, maxLen, "%s", (unsigned char *)resp);
  //  Copy ten led colors from leds[src .. src+9] to leds[dest .. dest+9]
          memmove( buffer, &resp, BITMAP_SIZE );
//          dumpCanvasPixels(buffer);
//          Serial.printf("buffer: %s\n", buffer);
          return res;
      });
    response->addHeader("Cache-Control", "no-cache");
    //response->addHeader("Content-Disposition", "attachment; filename=" + String("download.txt"));
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
      DynamicJsonDocument root(128);
      DeserializationError err = deserializeJson(root, (const char*)data);
      if (err == DeserializationError::Ok) {
        // Get a reference to the root object
        JsonObject obj = root.as<JsonObject>();
        if (obj.containsKey("string")) {
          _runningString = obj["string"].as<String>();//.as<char>();
        }
        if (obj.containsKey("speed")) {
          _runningStringSpeed = obj["speed"].as<int>() | _runningStringSpeed;//.as<char>();
          scrollTimer.setInterval(_runningStringSpeed);
        }
        if (obj.containsKey("active")) {
          _runningStringActive = obj["active"].as<bool>();// | _runningStringActive;//.as<char>();
          scrollTimer.pause ( _runningStringActive );
        }
        Serial.printf("str = %s, spd = %u, status = %u", _runningString.c_str(), _runningStringSpeed, _runningStringActive );
  
        request->send(200, "text/plain", "ack");
      } else
        request->send(404, "text/plain", ":(");
    });

server.on("/draw", HTTP_POST, [](AsyncWebServerRequest *request){
    //nothing and dont remove it
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/draw: len=%u, idx=%u, ttl=%u\n", len, index, total);
    int idx = 0;
    for ( int y = 0; y < MX_HEIGHT; y++ ){
      for ( int x = 0; x < MX_WIDTH; x++ ){
        idx = (x + y * MX_WIDTH) * 3;
  //      if ( 720 < bitmap[idx] + bitmap[idx+1]  + bitmap[idx+2] ){
  //        leds[ XY(x, y)] = CRGB::Black;
  //      } else {
          if ( ( data[idx] >= BG_COLOR_THRESH_MAX  ) && ( data[idx+1]  >= BG_COLOR_THRESH_MAX  ) && ( data[idx+2]  >= BG_COLOR_THRESH_MAX ) ){
            bitmap[idx] = 0x00;
            bitmap[idx+1] = 0x00;
            bitmap[idx+2] = 0x00;            
          } else {
            bitmap[idx] = data[idx];
            bitmap[idx+1] = data[idx+1];
            bitmap[idx+2] = data[idx+2];            
          }
          Serial.printf("%3u,%3u,%3u  ", data[idx], data[idx + 1], data[idx + 2]);
//        }
      }
      Serial.println();
    }
    fillCanvas(data);
    request->send(200, "text/plain", "ack");
  });



  

server.on("/pixel", HTTP_POST, [](AsyncWebServerRequest *request){
    //nothing and dont remove it
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("/pixel:\n");
    DynamicJsonDocument root(128);
//      JsonObject& root = jsonBuffer.parseObject((const char*)data);
    DeserializationError err = deserializeJson(root, (const char*)data);
    if (err == DeserializationError::Ok) {
      // Get a reference to the root object
      JsonObject obj = root.as<JsonObject>();

      unsigned char x = obj["x"];//.as<char>();
      unsigned char y = obj["y"];//.as<char>();
      unsigned char r = obj["r"];//.as<char>();
      unsigned char g = obj["g"];//.as<char>();
      unsigned char b = obj["b"];//.as<char>();

      Serial.printf("(%u:%u) r: %u g: %u b: %u\n\n", x, y, r, g, b);
        
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
