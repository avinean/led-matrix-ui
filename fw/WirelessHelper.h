#ifndef _WIRELESSHELPER_H_
#define _WIRELESSHELPER_H_

// Import required libraries
#include "WiFi.h"
#include <ESPmDNS.h>

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
  Serial.printf("\nmDNS responder started: http://%s.local\n", host);
  sprintf(__BOOT_STR__,"%d.%d.%d.%d;%s", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3], ssid );
}

#endif
