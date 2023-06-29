/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
#define BLYNK_TEMPLATE_ID "TMPL2wO-7VsYy"
#define BLYNK_TEMPLATE_NAME "IoB Dispenser"
#define PRODUCT_WIFI_SSID "IoB Dispenser"
#define BLYNK_FIRMWARE_VERSION "0.1.3"

#define BLYNK_PRINT Serial
// #define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
// #define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
// #define USE_WITTY_CLOUD_BOARD
// #define USE_WEMOS_D1_MINI

#include <BlynkEdgent.h>
#include <drink_dispenser_control.h>

int dispenseVolume=10;
bool rateLimit=false;

void setup()
{
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  ledProgram(0);
  digitalWrite(solenoidPin, LOW);  // close the solenoid valve
}

void loop()
{
  BlynkEdgent.run();
  if (digitalRead(D2) == HIGH)
  {
    Serial.print("Dispense button pressed");
    if (0 < dispenseVolume < 25 && rateLimit == false)
    {
      // dispense volume
      dispenseBeverage(dispenseVolume);
    }
    else if (0 < dispenseVolume < 25 && rateLimit == true){
      // rate limit exceeded
      //Blynk.logEvent("dispenseAttempt", String("Thirsty fuck got rate limited trying to dispense ") + dispenseVolume + String("oz"));
    }
  }
}