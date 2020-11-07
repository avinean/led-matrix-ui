#ifndef _OTAHELPER_H_
#define _OTAHELPER_H_

#include "SPIFFS.h"
#include <Update.h>

const char upload_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<HTML>
  <HEAD>
    <TITLE>Firare upload example</TITLE>
  </HEAD>
  <BODY>
    <H1>Choose .ino.bin file</H1>
    <form id="uploadform" enctype="multipart/form-data" method="post" action="/upload">
       <input id="fileupload" name="inobinfile" type="file" />
       <input type="submit" value="submit" id="submit" />
    </form>
  </BODY>
</HTML>
)=====";

void spiffsListDir(){
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
}

void printSPIFFSInfo(){
  int tBytes = SPIFFS.totalBytes();
  int uBytes = SPIFFS.usedBytes();
  Serial.printf("SPIFFS Info:\nTotal Bytes: %d\nUsed Bytes: %d\nFree Bytes: %d\n", tBytes, uBytes, ( tBytes - uBytes ) );
}

void updateFW(){
  File file = SPIFFS.open("/fw.bin");
  if(file){
    Serial.println("Got /fw.bin starting update process");
//    Serial.println("Starting update..");        
    
    size_t fileSize = file.size();
    
    if(!Update.begin(fileSize)){       
       Serial.println("Cannot do the update");
       return;
    };
    
    Update.writeStream(file);
    
    if(Update.end()){       
      Serial.println("Successful update");  
    }else {       
      Serial.println("Error Occurred: " + String(Update.getError()));
      return;
    }
     
    file.close();
    SPIFFS.remove("/fw.bin");
    
    Serial.println("Reset in 4 seconds...");
    delay(4000);
    
    ESP.restart();
      
  } else {
    Serial.println("No /fw.bin for update");
  }  
}

 #endif
