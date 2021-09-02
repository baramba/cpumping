#include <cstddef>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <FTPServer.h>
#include <TickerScheduler.h>

#ifndef pumper_h 
#define pumper_h 

String relay_switch();
String relay_status();
void relay_start(int time,int delay);


String getContentType(String filename);

bool handleFileRead(String path);

void httpInit();

extern byte RELAY; //GPIO4
extern ESP8266WebServer HTTP;
extern TickerScheduler taskSch;

#endif