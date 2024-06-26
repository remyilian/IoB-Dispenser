#ifndef DRINK_DISPENSER_CONTROL_H
#define DRINK_DISPENSER_CONTROL_H

#include <Adafruit_NeoPixel.h>
#include <RGBLed.h>

// Pin configurations
#define stripPin D4 // LED strip
#define numPixels 8 // Number of WS2812 LEDs
#define solenoidPin D0
#define buttonPin D2
#define redButtonLED D5
#define greenButtonLED D6
#define blueButtonLED D7

// flow meter
volatile int pulseCount = 0;   // Variable to count the pulses from the flow meter
float flowRate = 0.0;          // Flow rate in liters per minute
float totalLiters = 0.0;       // Total dispensed liters
// Constants for flow meter calibration
const float calibrationFactor = 5880.0;    // Pulses per liter
const float ouncesPerLiter = 33.814;       // Ounces per liter

Adafruit_NeoPixel strip(numPixels, stripPin, NEO_GRB + NEO_KHZ800);
RGBLed buttonLED(redButtonLED, greenButtonLED, blueButtonLED, RGBLed::COMMON_CATHODE);

void initializePins() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to off
  digitalWrite(solenoidPin, LOW);  // close the solenoid valve
}

void updateStripColor(int red, int green, int blue) {
  for (int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  //strip.show();
}

void ledProgram(int program){
  switch (program)
  {
  //basic led for init and stuff
  case 0:
    for (int i = 0; i < numPixels; i++){
      strip.setPixelColor(i, 255, 255, 255);
      delay(300);
      strip.show();
    }
    buttonLED.fadeIn(255,255,255,20,3);
    strip.show();
    break;
  //led off
  case 1:
    strip.fill();
    buttonLED.off();
    break;
  //rate limit
  case 2:
    buttonLED.flash(RGBLed::RED, 250);
    strip.fill();
    break;
  //dispense
  case 3:
    buttonLED.crossFade(RGBLed::GREEN,RGBLed::BLUE,20,2500);
    updateStripColor(64,64,64);
    strip.setPixelColor(2, 255, 255, 255);
    strip.setPixelColor(5, 255, 255, 255);
    strip.show();
    break;
  //return to ready
  case 4:
    buttonLED.crossFade(RGBLed::BLUE,RGBLed::WHITE,20,2500);
    updateStripColor(64,64,64);
    strip.show();
    break;
  }

}

//dispense beverage. no respect for rate limiting
void dispenseBeverage(float ounces) {
  const unsigned long timeout = 10000;  // Timeout duration in milliseconds
  unsigned long startTime = millis();   // Record the starting time

  float litersToDispense = ounces / ouncesPerLiter;
  float pulsesToDispense = litersToDispense * calibrationFactor;
  Blynk.virtualWrite(V2, 1);
  pulseCount = 0;
  totalLiters = 0.0;
  digitalWrite(solenoidPin, HIGH);  // Open the solenoid valve
  Serial.println("Solenoid opened");
  //Blynk.logEvent("dispenseAttempt", String("Dispensing ") + ounces + String("oz"));
  ledProgram(3);
  
  while (pulseCount < pulsesToDispense) {
    flowRate = pulseCount / (millis() / 60000.0);
    totalLiters = pulseCount / calibrationFactor;
    
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.print(" L/min");
    
    Serial.print("  Total Dispensed: ");
    Serial.print(totalLiters * ouncesPerLiter);
    Serial.println(" oz");
    
    delay(500);

    // Check if the timeout has been reached
    if (millis() - startTime >= timeout) {
      Serial.println("Dispensing timed out");
      break;  // Exit the while loop and return from the function
    }
  }
  
  //Blynk.logEvent("dispenseAttempt", totalLiters + String("L of ") + litersToDispense + String("L dispensed"));
  digitalWrite(solenoidPin, LOW);   // Close the solenoid valve
  Serial.println("Solenoid closed");
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
    Serial.println("Blynk dispense");
  }
}

BLYNK_WRITE(V2){
    if(param.asInt() == 1){
      digitalWrite(solenoidPin, HIGH);   // Open the solenoid valve
      Serial.println("Solenoid opened from Blynk");
    }
    else if(param.asInt() == 0){
      digitalWrite(solenoidPin, LOW);   // Close the solenoid valve
      Serial.println("Solenoid closed from Blynk");
    }
}
#endif
