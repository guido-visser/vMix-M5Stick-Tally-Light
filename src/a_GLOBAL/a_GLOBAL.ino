#include <WiFi.h>
#include <M5StickC.h>
#include <PinButton.h>
#include <WebServer.h>
#include <Preferences.h>

#define LED_BUILTIN 10

Preferences preferences;

//USER CONFIGURATION -- Fill these in
String WIFI_SSID = "";
String WIFI_PASS = "";
String VMIX_IP = "";
int VMIX_PORT = 8099;
int TALLY_NR = 1;
// END CONFIGURATION
