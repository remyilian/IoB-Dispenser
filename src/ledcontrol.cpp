#define NUM_LEDS 12
#define DATA_PIN 2 
#define COLOR_ORDER GRB //Green (G), Red (R), Blue (B)
#define CHIPSET WS2812
#define BRIGHTNESS 60
#define VOLTS 5
#define MAX_AMPS 500 //value in milliamps

//ENOUGH NONSENSE!

CRGB leds[NUM_LEDS];

void fastled_init() {
//configure FastLED Library
FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(courtesy,NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
FastLED.setBrightness(BRIGHTNESS);
FastLED.clear();
FastLED.show(); 
}

void courtesy_set(start,stop,h,s,v) {
    courtesy[start,stop] = CHSV(h,s,v);
    FastLED.show();
}

void (){
    switch (effect)
    {
    case bright:
        courtesy_set(0,11,255,255,255);
        break;
    
    default:
        break;
    }
}