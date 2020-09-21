#include <WiFi.h>
//#include <M5StickC.h>
#include <M5StickCPlus.h>
#include <PinButton.h>
#include <WebServer.h>
#include <Preferences.h>
#include "k_PLUGINMANAGER.h";

/*
  Set the variable below to 1 when using the M5StickC-Plus
  When you use the PLUS, comment the #include <M5StickC.h> line at the top of this file by adding // to the front of the line
  and uncomment the #include <M5StickCPlus.h> file by removing //
*/
bool C_PLUS = 1;

#define LED_BUILTIN 10

Preferences preferences;

//DON'T CHANGE THESE VARIABLES, YOU CAN CHANGE THEM IN THE WEB UI
String WIFI_SSID = "";
String WIFI_PASS = "";
String VMIX_IP = "";
String M_TALLY = "";
int VMIX_PORT = 8099; //USES THE TCP API PORT, THIS IS FIXED IN VMIX
int TALLY_NR = 1;
int BRIGHTNESS = 12; //100%
