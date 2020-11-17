#ifndef _SPIFFS_FUNCTIONS_H_
#define _SPIFFS_FUNCTIONS_H_

/*====================================================================================
  This sketch contains support functions for the ESP6266 SPIFFS filing system

  Created by Bodmer 15th Jan 2017
  Updated by Bodmer to support ESP32 with SPIFFS Jan 2018
  ==================================================================================*/

//#include  "ListLib.h"



void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

//====================================================================================
//                 Print a SPIFFS directory list (root directory)
//====================================================================================
#ifdef ESP8266
void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

  fs::Dir dir = SPIFFS.openDir("/"); // Root directory
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 21 - fileName.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");

    fs::File f = dir.openFile("r");
    String fileSize = (String) f.size();
    spaces = 10 - fileSize.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");
    Serial.println(fileSize + " bytes");
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
}
#endif

//====================================================================================

#ifdef ESP32

void dumpFile(const char* fname){
  File f = SPIFFS.open(fname, "r");
  if (f && f.size()) {
      Serial.println("Dumping log file");
            
      String debugLogData;
  
      while (f.available()){
        debugLogData += char(f.read());
      }
      f.close();
           
      Serial.println("=====================================");
      Serial.println(debugLogData);
      Serial.println("=====================================");
  }
}


void readFromFile(const char* fname, const uint8_t* fdst, uint16_t fsize){
  File file = SPIFFS.open(fname, "r");
 
  if (!file) {
    Serial.println("Error opening file for reading");
    return;
  }
 
  int bytesReaded = file.read((uint8_t*)fdst, fsize);//print("TEST SPIFFS");
 
  if (bytesReaded > 0) {
    Serial.println("File was Readed");
    Serial.println(bytesReaded);
 
  } else {
    Serial.println("File read failed");
  }
 
  file.close();
}

void writeToFile(const char* fname, const uint8_t* fdata, uint16_t fsize){
  File file = SPIFFS.open(fname, "w");
 
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }

int bytesWritten = 0;
 do{
  bytesWritten += file.write(fdata, fsize);//print("TEST SPIFFS");
 } while(fsize > bytesWritten);
 
  if (bytesWritten == fsize ) {
    Serial.println("File was written");
    Serial.println(bytesWritten);
 
  } else {
    Serial.println("File write failed");
  }
 
  file.close();
}

void listFiles(void) {
  listDir(SPIFFS, "/", 0);
}



void findFilesInDir(fs::FS &fs, const char * dirname, const char * fileExt, uint8_t levels, doOnFileFoundCallback callback) {

  fs::File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;// res;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;// res;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.println(fileName);
      if (levels) {
        findFilesInDir(fs, file.name(), fileExt, levels - 1, callback);
      }
    } else {
      if ( String(file.name()).endsWith( fileExt ) ){
        Serial.println("  " + String(file.name()));
        callback(file.name());
      }
    }

    file = root.openNextFile();
  }
//  return res;
}

void printSPIFFSInfo(){
  int tBytes = SPIFFS.totalBytes();
  int uBytes = SPIFFS.usedBytes();
  Serial.printf("SPIFFS Info:\nTotal Bytes: %d\nUsed Bytes: %d\nFree Bytes: %d\n", tBytes, uBytes, ( tBytes - uBytes ) );
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {

  Serial.println();
  printSPIFFSInfo();
  Serial.println("SPIFFS files found:");

  Serial.printf("Listing directory: %s\n", "/");
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  fs::File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      int spaces = 20 - fileName.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      String fileSize = (String) file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
}
#endif

#endif
