#include <WiFi.h>
#include <M5StickC.h>
#include <PinButton.h>
#include <WebServer.h>
#include <Preferences.h>

#define LED_BUILTIN 10

//USER CONFIGURATION -- Fill these in
#define WIFI_SSID ""
#define WIFI_PASS ""
#define VMIX_IP ""
#define VMIX_PORT 8099
int TALLY_NR = 1;
// END CONFIGURATION
