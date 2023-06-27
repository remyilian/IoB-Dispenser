#ifndef DRINK_DISPENSER_CONTROL_H
#define DRINK_DISPENSER_CONTROL_H

#include <Adafruit_NeoPixel.h>
#include <RGBLed.h>


const int solenoidPin = D0;
// flow meter
volatile int pulseCount = 0;   // Variable to count the pulses from the flow meter
float flowRate = 0.0;          // Flow rate in liters per minute
float totalLiters = 0.0;       // Total dispensed liters
// Constants for flow meter calibration
const float calibrationFactor = 5880.0;    // Pulses per liter
const float ouncesPerLiter = 33.814;       // Ounces per liter


// Pin configuration for the WS2812 strip
const int stripPin = D4;
const int numPixels = 8; // Number of WS2812 LEDs

// Pin configuration for the status LEDs
const int redButtonLED = D5;
const int greenButtonLED = D6;
const int blueButtonLED = D7;

Adafruit_NeoPixel strip(numPixels, stripPin, NEO_GRB + NEO_KHZ800);
RGBLed buttonLED(redButtonLED, greenButtonLED, blueButtonLED, RGBLed::COMMON_CATHODE);

void initializePins() {
  pinMode(buttonLedPin1, OUTPUT);
  pinMode(buttonLedPin2, OUTPUT);
  pinMode(buttonLedPin3, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to off
}

void updateStripColor(int red, int green, int blue) {
  for (int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  //strip.show();
}

void ledProgram(int program){
  switch (expression)
  {
  //basic led for init and stuff
  case program==0
    for (int i = 0; i < numPixels; i++){
      strip.setPixelColor(i, 255, 255, 255);
      wait(300);
      strip.show();
    }
    buttonLED.fadeIn(255,255,255,20,3);
    strip.show();
    break;
  //led off
  case program==1
    strip.fill();
    buttonLED.off();
    break;
  //rate limit
  case program==2
    buttonLED.flash(RGBLed::RED, 250);
    strip.fill();
    break;
  //dispense
  case program==3
    buttonLED.crossFade(RGBLed::GREEN,RGBLed::BLUE,20,2500);
    updateStripColor(64,64,64);
    strip.setPixelColor(2, 255, 255, 255);
    strip.setPixelColor(5, 255, 255, 255);
    strip.show();
    break;
  //return to ready
  case program==4
    buttonLED.crossFade(RGBLed::BLUE,RGBLed::WHITE,20,2500);
    updateStripColor(64,64,64);
    strip.show();
    break;
  }

}

//dispense beverage. no respect for rate limiting
void dispenseBeverage(float ounces) {
  float litersToDispense = ounces / ouncesPerLiter;
  float pulsesToDispense = litersToDispense * calibrationFactor;
  Blynk.virtualWrite(V2, 1);
  pulseCount = 0;
  totalLiters = 0.0;
  
  digitalWrite(solenoidPin, HIGH);  // Open the solenoid valve
  Blynk.logEvent("dispenseAttempt", String("Dispensing ") + ounces + String("oz"));
  ledProgram(3);
  while (pulseCount < pulsesToDispense) {
    flowRate = pulseCount / (millis() / 60000.0);
    totalLiters = pulseCount / calibrationFactor;
    
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.print(" L/min");
    
    Serial.print("  Total Dispensed: ");
    Serial.print(totalLiters*ouncesPerLiter);
    Serial.println(" oz");

    delay(500);  // Adjust delay as per your requirements
  }
  Blynk.logEvent("dispenseAttempt", totalLiters + String("L of ") + litersToDispense + String("L dispensed"));
  digitalWrite(solenoidPin, LOW);   // Close the solenoid valve
  Blynk.virtualWrite(V2, 0);
  pulseCount = 0;
  totalLiters = 0.0;

  ledProgram(4);
  Serial.println("Dispensing complete");
}
//when the dispense volume changes
BLYNK_WRITE(V0){
  //if dispense volume is in range. blynk has more value filtering as well.
  if(0<param.asInt()<25){
    //dispense volume 
    dispenseBeverage(param.asInt());
  }
}
#endif
