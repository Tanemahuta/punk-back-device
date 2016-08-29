#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include "LED.h"
#include "SwitchHandler.h"
#include "GoogleForms.h"

/** LEDs */
LED ledReady(D0);
LED ledError(D1);

/** Switches */
Switch poor(D4);
Switch fair(D5);
Switch good(D6);
Switch excellent(D7);

const Switch switches[] = {poor, fair, good, excellent};
SwitchHandler switchHandler(switches, 4);

WiFiClientSecure wifiClient;
GoogleForms gForms(wifiClient);

//const char* deviceName = "feedback-1";

/** Wifi configuration */
const char* ssid = "Tanemahuta";
const char* password = "7ykcRytmjs6k";

enum State {
  FEEDBACK, FEEDBACK_ERROR, SERVER
};

State currentState = FEEDBACK;

/*
  const char* ssid = "Namics";
  const char* password = "NamicsSeit1995";
*/

/** Google form configuration **/
/*
  const char* formUrl = "https://docs.google.com/a/namics.com/forms/d/e/1FAIpQLScgbUAViqBmxZab-M3T3rkygjwbnW5FzgvCZoL0gLKtDh6Bzw";
  const char* formEntry = "entry.74407846";
  WiFiClientSecure client;
  GoogleForms forms(client);
*/

/**
   Wifi connection method.
*/
void connectToWifi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("...");

  WiFi.begin(ssid, password);
  ledReady.blink(500);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    ledReady.handle();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("connected.");
    ledReady.on();
  } else {
    Serial.println("failed.");
    ledError.blink(200);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  connectToWifi();
}

bool isServerSetupComboPressed() {
  return switchHandler.getPressedCount() == 2 &&
         switchHandler.isPressed(0) &&
         switchHandler.isPressed(switchHandler.getSwitchesCount() - 1);
}

void handleFeedback() {
  switchHandler.refresh();
  if (isServerSetupComboPressed()) {
    Serial.println("Server setup combo pressed, creating http server");
    currentState = SERVER;
    ledReady.on();
    ledError.blink(1000);
    return;
  } else if (switchHandler.getPressedCount() > 1) {
    Serial.print("More than one button (");
    Serial.print(switchHandler.getPressedCount());
    Serial.print(": ");
    for (int i = 0; i < switchHandler.getSwitchesCount(); i++) {
      if (switchHandler.isPressed(i)) {
        Serial.print(i);
      }
    }    
    Serial.println(") pressed, waiting for all to be released.");
    currentState = FEEDBACK_ERROR;
    ledError.blink(200);
    ledReady.off();
  } else {
    for (int i = 0; i < switchHandler.getSwitchesCount(); i++) {
      if (switchHandler.wasReleased(i)) {
        Serial.print("Found feedback button ");
        Serial.print(i);
        Serial.println(" to be released, sending feedback.");
        gForms.sendFeedback(i);
      }
    }
  }
  delay(100);
}

void handleFeedbackError() {
  switchHandler.refresh();
  if (switchHandler.getPressedCount() == 0) {
    currentState = FEEDBACK;
    ledReady.on();
    ledError.off();
  }
  delay(100);
}

void handleServer() {

}

void loop() {
  switch (currentState) {
    case FEEDBACK: handleFeedback(); break;
    case FEEDBACK_ERROR: handleFeedbackError(); break;
    case SERVER: handleServer(); break;
  }
  ledReady.handle();
  ledError.handle();
}

