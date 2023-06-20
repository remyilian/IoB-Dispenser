#define BLYNK_TEMPLATE_ID "TMPL2wO-7VsYy"
#define BLYNK_TEMPLATE_NAME "IoB Dispenser"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD

#include <BlynkEdgent.h>
#include <Arduino.h>
#include <FastLED.h>

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
  fastled_init();
}

void loop() {
  BlynkEdgent.run();
}
