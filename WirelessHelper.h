#ifndef _WIRELESSHELPER_H_
#define _WIRELESSHELPER_H_

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include "OTAHelper.h"

#include <ESPmDNS.h>


#include "AsyncJson.h"
#include "ArduinoJson.h"

const char* host = "esp32";
const char* ssid = "Keenetic-1933";
const char* password = "sMTVCwBP";

// Create AsyncWebServer object on port 80
//AsyncWebServer server(80);

void setupWifiConnection(){
    // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

#endif
