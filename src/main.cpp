#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <drink_dispenser_control.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

int selectedQuantity = 10; // Default quantity to dispense

// Rest of your code...

//rate limiting
unsigned int maxDispensesPerPeriod = 5;  // Maximum number of dispenses allowed per period
unsigned int dispensesRemaining = maxDispensesPerPeriod;  // Number of remaining dispenses
unsigned long dispensingPeriod = 60000;  // Time period in milliseconds (e.g., 1 minute)
unsigned long lastDispenseTimestamp = 0;  // Timestamp of the last dispense


void handleRoot() {
  String webpage = "<html><body>";
  webpage += "<h1>Smart Drink Dispenser</h1>";
  webpage += "<p>Select the quantity to dispense:</p>";
  webpage += "<select onchange=\"updateSelectedQuantity(this.value)\">";
  webpage += "<option value=\"10\">10 ounces</option>";
  webpage += "<option value=\"20\">20 ounces</option>";
  webpage += "<option value=\"30\">30 ounces</option>";
  webpage += "</select><br><br>";
  webpage += "<button onclick=\"dispenseBeverage()\">Dispense Drink</button>";
  webpage += "<script>function updateSelectedQuantity(quantity) {";
  webpage += "selectedQuantity = parseInt(quantity);}";
  webpage += "function dispenseBeverage() {";
  webpage += "var xhttp = new XMLHttpRequest();";
  webpage += "xhttp.open('GET', '/dispense', true);";
  webpage += "xhttp.send();}</script>";
  webpage += "</body></html>";

  server.send(200, "text/html", webpage);
}


void handleDispense() {
  unsigned long currentTimestamp = millis();

  // Check if enough time has passed since the last dispense
  if (currentTimestamp - lastDispenseTimestamp < dispensingPeriod) {
    unsigned long timeRemaining = dispensingPeriod - (currentTimestamp - lastDispenseTimestamp);
    String response = "Please wait ";
    response += timeRemaining / 1000;
    response += " seconds before dispensing another drink.";
    server.send(429, "text/plain", response);
    return;
  }

  // Check if the maximum number of dispenses for the period has been reached
  if (dispensesRemaining <= 0) {
    server.send(429, "text/plain", "Dispense limit reached for this period.");
    return;
  }

  // Update the rate limiting variables
  lastDispenseTimestamp = currentTimestamp;
  dispensesRemaining--;

  // Perform the drink dispensing actions
  updateButtonLEDs(true);          // Turn on the button LEDs
  dispenseBeverage(selectedQuantity);  // Dispense the selected quantity of beverage
  updateButtonLEDs(false);         // Turn off the button LEDs

  // Send the success response
  server.send(200, "text/plain", "Drink dispensed");
}

void setup() {
  // Rest of your setup...

  initializeLEDs();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Start the web server
  server.on("/", handleRoot);
  server.on("/dispense", handleDispense);
  server.begin();
}

void loop() {
  // Rest of your loop...

  server.handleClient();

  // Rest of your loop...
}