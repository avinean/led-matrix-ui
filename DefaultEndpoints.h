#ifndef _DEFAULTENDPOINTS_H_
#define _DEFAULTENDPOINTS_H_

void setupDefaultEndpoints ( AsyncWebServer* server ){
  // Route for root / web page
  server->on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Route to load style.css file
  server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
 
  // Route to load style.css file
  server->on("/animations.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/animations.js", "text/javascript");
  });
 
  // Route to load style.css file
  server->on("/animationUpload.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/animationUpload.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/appCreator.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/appCreator.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/appMounter.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/appMounter.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/effects.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/effects.js", "text/javascript");
  });    

// Route to load style.css file
  server->on("/pictureDrawer.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/pictureDrawer.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/pictureGallery.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/pictureGallery.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/picture.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/picture.js", "text/javascript");
  });

// Route to load style.css file
  server->on("/pictureUpload.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/pictureUpload.js", "text/javascript");
  });    

// Route to load style.css file
  server->on("/text.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/text.js", "text/javascript");
  });          

// Route to load style.css file
  server->on("/observer.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/observer.js", "text/javascript");
  });            
// Route to load style.css file
  server->on("/services.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/services.js", "text/javascript");
  });              
// Route to library for prsing gifs
  server->on("/libs/libgif.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/libs/libgif.js", "text/javascript");
  });              
}

#endif //_DEFAULTENDPOINTS_H_
