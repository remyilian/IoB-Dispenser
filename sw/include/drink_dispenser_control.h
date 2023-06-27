#ifndef DRINK_DISPENSER_CONTROL_H
#define DRINK_DISPENSER_CONTROL_H

#include <Adafruit_NeoPixel.h>

// Pin configuration for the status LEDs
const int buttonLedPin1 = D5;
const int buttonLedPin2 = D6;
const int buttonLedPin3 = D7;

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

Adafruit_NeoPixel strip(numPixels, stripPin, NEO_GRB + NEO_KHZ800);

void initializePins() {
  pinMode(buttonLedPin1, OUTPUT);
  pinMode(buttonLedPin2, OUTPUT);
  pinMode(buttonLedPin3, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to off
}

void updateButtonLEDs(bool isActive) {
  digitalWrite(buttonLedPin1, isActive);
  digitalWrite(buttonLedPin2, isActive);
  digitalWrite(buttonLedPin3, isActive);
}

void updateStripColor(int red, int green, int blue) {
  for (int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}

void dispenseBeverage(float ounces) {
  float litersToDispense = ounces / ouncesPerLiter;
  float pulsesToDispense = litersToDispense * calibrationFactor;
  
  pulseCount = 0;
  totalLiters = 0.0;
  
  digitalWrite(solenoidPin, HIGH);  // Open the solenoid valve
  Blynk.logEvent("dispenseAttempt", String("Dispensing ") + ounces + String("oz"));
  while (pulseCount < pulsesToDispense) {
    flowRate = pulseCount / (millis() / 60000.0);
    totalLiters = pulseCount / calibrationFactor;
    
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.print(" L/min");
    
    Serial.print("  Total Dispensed: ");
    Serial.print(totalLiters);
    Serial.println(" L");

    delay(500);  // Adjust delay as per your requirements
  }
  Blynk.logEvent("dispenseAttempt", totalLiters + String("L of ") + litersToDispense + String("L dispensed"));
  digitalWrite(solenoidPin, LOW);   // Close the solenoid valve
  pulseCount = 0;
  totalLiters = 0.0;

  Serial.println("Dispensing complete");
}
//when the dispense volume changes
BLYNK_WRITE(V0){
  //this could be a switch
  if(0<param.asInt()<25){
    //dispense volume 
    dispenseBeverage(param.asInt());
  }
}
#endif
