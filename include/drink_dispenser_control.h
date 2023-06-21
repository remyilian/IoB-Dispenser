#ifndef DRINK_DISPENSER_CONTROL_H
#define DRINK_DISPENSER_CONTROL_H

#include <Adafruit_NeoPixel.h>

// Pin configuration for the status LEDs
const int buttonLedPin1 = D5;
const int buttonLedPin2 = D6;
const int buttonLedPin3 = D7;

// Pin configuration for the WS2812 strip
const int stripPin = D4;
const int numPixels = 8; // Number of WS2812 LEDs

Adafruit_NeoPixel strip(numPixels, stripPin, NEO_GRB + NEO_KHZ800);

void initializeLEDs() {
  pinMode(buttonLedPin1, OUTPUT);
  pinMode(buttonLedPin2, OUTPUT);
  pinMode(buttonLedPin3, OUTPUT);

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

#endif
