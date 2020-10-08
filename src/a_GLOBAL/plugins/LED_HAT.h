#include "FastLED.h"

#define LED_BRIGHTNESS     20    //VALUE FROM 0 TO 255
#define DATA_PIN    32
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    37
CRGB leds[NUM_LEDS];

class M5TallyLEDHat{

public:
	M5TallyLEDHat(){
		FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  	FastLED.setBrightness(LED_BRIGHTNESS);
	}

	void onLive() {
		fill_solid( leds, NUM_LEDS, CRGB::Red);
  	FastLED.show();
	}

	void onPre() {
    fill_solid( leds, NUM_LEDS, CRGB::Green);
		FastLED.show();
  }

  void onSafe() {
    FastLED.clear();
		FastLED.show();
  }

  void onClear() {
  	FastLED.clear();
		FastLED.show();
  }

};