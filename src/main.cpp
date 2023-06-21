#include <Arduino.h>
#include <drink_dispenser_control.h>

const int buttonPin = D2;      // Pin connected to the button
const int solenoidPin = D0;    // Pin connected to the solenoid valve
const int flowMeterPin = D1;   // Pin connected to the flow meter

volatile int pulseCount = 0;   // Variable to count the pulses from the flow meter
float flowRate = 0.0;          // Flow rate in liters per minute
float totalLiters = 0.0;       // Total dispensed liters

// Constants for flow meter calibration
const float calibrationFactor = 5880.0;    // Pulses per liter
const float ouncesPerLiter = 33.814;       // Ounces per liter

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(solenoidPin, OUTPUT);
  pinMode(flowMeterPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(flowMeterPin), pulseCounter, FALLING);

  Serial.begin(115200);
  initializeLEDs();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    updateButtonLEDs(true);    // Turn on the button LEDs
    dispenseBeverage(10);      // Dispense 10 ounces of beverage
    updateButtonLEDs(false);   // Turn off the button LEDs
    delay(2000);               // Delay to prevent button bouncing
  }
}

void dispenseBeverage(float ounces) {
  float litersToDispense = ounces / ouncesPerLiter;
  float pulsesToDispense = litersToDispense * calibrationFactor;
  
  pulseCount = 0;
  totalLiters = 0.0;
  
  digitalWrite(solenoidPin, HIGH);  // Open the solenoid valve

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

  digitalWrite(solenoidPin, LOW);   // Close the solenoid valve
  pulseCount = 0;
  totalLiters = 0.0;

  Serial.println("Dispensing complete");
}
