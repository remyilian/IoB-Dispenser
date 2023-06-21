#include <Arduino.h>
#include <FastLED.h>
#include <FlowSensor.h>

//fastled memory
CRGB courtesy[12];
//flowmeter
FlowSensor FlowMeter(YFS201, D1);
//how much juice do you want
int BorgQty=10;

//idk the flowmeter lib says to do this with an esp
void IRAM_ATTR count()
{
  FlowMeter.count();
}


void setup()
{
  
  // setup ws2812, gpio2, grb order, name courtesy,12 leds
  FastLED.setMaxPowerInVoltsAndMilliamps(5,400); 
  FastLED.addLeds<WS2812, 2, GRB>(courtesy, 12);
  FastLED.clear();
  FastLED.show();
  //init flowmeter
  FlowMeter.begin(count);

  //set pins
  pinMode(D2, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(D2) == HIGH){
    dispense(BorgQty);
  }
}

void courtesy_set(int start,int stop,int h,int s,int v)
{
  courtesy[start,stop] = CHSV(h,s,v);
  FastLED.show();
}

void courtesy_fx(int effect)
{
  switch (effect)
  {
  case 0:
    courtesy_set(0, 11, 0, 0, 0);
    break;
  case 1:
    courtesy_set(0, 11, 255, 255, 255);
    break;
  case 2:
    courtesy_set(2, 3, 255, 255, 64);
    courtesy_set(5, 6, 255, 255, 64);
    break;

  default:
    courtesy_set(0, 11, 0, 0, 0);
    break;
  }
}

float get_volume(){
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