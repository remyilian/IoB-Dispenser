//#define BLYNK_TEMPLATE_ID "TMPL2wO-7VsYy"
//#define BLYNK_TEMPLATE_NAME "IoB Dispenser"
//#define BLYNK_FIRMWARE_VERSION "0.1.0"
//#define BLYNK_PRINT Serial
//#define USE_NODE_MCU_BOARD

//#include <BlynkEdgent.h>
#include <Arduino.h>
#include <FastLED.h>
#include <FlowSensor.h>

//fastled
CRGB courtesy[12];


FlowSensor FlowMeter(YFS201, D1);

void IRAM_ATTR count()
{
  FlowMeter.count();
}


void setup()
{
  
  //BlynkEdgent.begin();
  fastled_init();
  FlowMeter.begin(count);
}

void loop()
{
  //BlynkEdgent.run();
}

// void fastledinit()
// {
//   // set up ws2812, gpio2, grb order, name courtesy,12 leds
//   FastLED.setMaxPowerInVoltsAndMilliamps(5,400); 
//   FastLED.addLeds<WS2812, 2, GRB>(courtesy[12], 12);
//   FastLED.clear();
//   FastLED.show();
// }

void courtesyset(start, stop, h, s, v)
{
  courtesy[start, stop] = CHSV(h, s, v);
  FastLED.show();
}

void courtesy_fx(effect)
{
  switch (effect)
  {
  case bright:
    courtesy_set(0, 11, 255, 255, 255);
    break;
  case dim:
    courtesy_set(2, 3, 255, 255, 64);
    courtesy_set(5, 6, 255, 255, 64);
    break;
    case off:
    courtesy_set(0, 11, 0, 0, 0);
    break;
  default:
    break;
  }
}

void get_volume(){
  		FlowMeter.read();
      return FlowMeter.getVolume();
}

void dispense(float volume){
  volume = volume/33.814;//convert volume in oz to liters that sensor will return
  //close valve and reset counter
  digitalWrite(D0,HIGH);
  FlowMeter.resetVolume();
  //dispense until target volume has flowed
  while(volume>get_volume()){
    digitalWrite(D0,LOW); //open valve
  }
  //close valve and reset counter
  digitalWrite(D0,HIGH);
  FlowMeter.resetVolume();
}