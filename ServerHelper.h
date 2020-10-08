#ifndef _SERVERHELPER_H_
#define _SERVERHELPER_H_

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"


// SSID & Password
const char* ssid = "Keenetic-1933";  // Enter your SSID here
const char* password = "sMTVCwBP";  //Enter your Password here

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var) {
  Serial.println(var);
  if (var == "STATE") {
    if (digitalRead(ledPin)) {
      ledState = "ON";
    }
    else {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
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

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load style.css file
  server.on("/draw.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/draw.js", "text/javascript");
  });

  // Route to set GPIO to HIGH
  server.on("/draw", HTTP_POST, [](AsyncWebServerRequest * request) {
    //    digitalWrite(ledPin, HIGH);
    //List all parameters
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){ //p->isPost() is also true
        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        if ( p->name().equals( "data" )){
          int r=0, t=0;
          String oneLine = p->value();//.c_str();//"123;456;789;999;";
          Serial.printf("POST[%s]:\n", p->name().c_str());
          for (int i=0; i < oneLine.length(); i++)
          { 
           if(oneLine.charAt(i) == ',') 
            { 
              bitmap[t] = oneLine.substring(r, i).toInt(); 
              Serial.printf("%d\n", bitmap[t]);
//              if ( i % LED_MATRIX_DEPTH )
//                Serial.printf("\n");
              r=(i+1); 
              t++; 
            }
          }
          bitmap[t] = oneLine.substring(r, oneLine.length()).toInt(); 
          Serial.printf("%d\n", bitmap[t]);          
        } else        
          Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

// Route for root / web page
  server.on("/drawer", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index1.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/pixel.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/pixel.js", "text/javascript");
  });

  // Route to set GPIO to HIGH
  server.on("/pixel", HTTP_POST, [](AsyncWebServerRequest * request) {
    //    digitalWrite(ledPin, HIGH);
    //List all parameters
    int params = request->params();
    if ( params == 5 ) {
      int x = request->getParam(0)->value().toInt();
      int y = request->getParam(1)->value().toInt();      
      int idx = (x + y * LED_MATRIX_WIDTH) * LED_MATRIX_DEPTH;
      bitmap[idx] = request->getParam(2)->value().toInt();
      bitmap[idx+1] = request->getParam(3)->value().toInt();
      bitmap[idx+2] = request->getParam(4)->value().toInt();
      
//      for(int i=0;i<params;i++){
//        AsyncWebParameter* p = request->getParam(i);
//        if(p->isPost()){        
//            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//        }
//      }
    }
    request->send(SPIFFS, "/index1.html", String(), false, processor);
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();  
}

#endif //_SERVERHELPER_H_
