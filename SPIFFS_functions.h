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

void listFiles(void) {
  listDir(SPIFFS, "/", 0);
}

typedef void (*doOnFileFoundCallback)(const char * dirname, const char * fileName);

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
      Serial.print(fileName);
      if (levels) {
        findFilesInDir(fs, file.name(), fileExt, levels - 1, callback);
      }
    } else {
      if ( String(file.name()).endsWith( fileExt ) ){
//        Serial.print("  " + String(file.name()));
        callback(root.name(), file.name());
//        String path = String(root.name());
//        String fn = String(file.name());
//        char s[path.length()+fn.length()+1];
//        sprintf(s, "%s/%s", path, fn);
//        String fileName = String(s);//root.name() + "/" + file.name();
//        res.Add(fileName);
//        Serial.print("  " + fileName);
//        int spaces = 20 - fileName.length(); // Tabulate nicely
//        while (spaces--) Serial.print(" ");
//        String fileSize = (String) file.size();
//        spaces = 10 - fileSize.length(); // Tabulate nicely
//        while (spaces--) Serial.print(" ");
//        Serial.println(fileSize + " bytes");
      }
    }

    file = root.openNextFile();
  }
//  return res;
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {

  Serial.println();
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
