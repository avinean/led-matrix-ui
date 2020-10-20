#ifndef _WIRELESS_HELPER_H_
#define _WIRELESS_HELPER_H_

bool createSoftAp(){
  Serial.printf("Creating SoftAP\n");
  WiFi.mode(WIFI_AP);                    // Changing ESP32 wifi mode to AccessPoint

  // You can remove the Appassword parameter if you want the hotspot to be open.
  WiFi.softAP(Apssid, Appassword);      //Starting AccessPoint on given credential
  myIP = WiFi.softAPIP();     //IP Address of our Esp8266 accesspoint(where we can host webpages, and see data)
  return true;  
}

bool connectToWifiAp( unsigned char retries){
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  do{
    delay(1000);
    Serial.printf("Connecting to WiFi..%d\n", retries);
  } while ( (--retries > 0 ) && (WiFi.status() != WL_CONNECTED) );  
  if ( WiFi.status() == WL_CONNECTED )
    myIP = WiFi.localIP();
  return WiFi.status() == WL_CONNECTED;
}

#endif  //_WIRELESS_HELPER_H_
