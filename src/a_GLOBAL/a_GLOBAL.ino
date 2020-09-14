#include <WiFi.h>
#include <M5StickC.h>
#include <PinButton.h>
#include <WebServer.h>
#include <Preferences.h>
#include "FastLED.h"    //NEEDED FOR NEOFLASH HAT

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

// NEOFLASH HAT LED STRIP VARIABLES (NOT NEEDED IF YOU DON'T HAVE THE HAT ON)
// YOU SHOULD ONLY NEED TO ADJUST NEOFLASH_BRIGHTNESS
#define NEOFLASH_BRIGHTNESS     46    //VALUE FROM 0 TO 255
#define DATA_PIN    26
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    126
CRGB leds[NUM_LEDS];
