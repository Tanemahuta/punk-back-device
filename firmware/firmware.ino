#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "LED.h"
#include "SwitchHandler.h"
#include "GoogleForms.h"
#include "WebServer.h"
#include "SettingsProvider.h"

#define WIFI_TIMEOUT_MILLIS 30*1000

/** LEDs */
LED ledReady(D0);
LED ledError(D1);

/** Switches */
SwitchHandler switchHandler(D4,D5,D6,D7);

/** Google forms handler */
GoogleForms gForms;

/** Settings provider for persistent settings */
SettingsProvider settings;

/**
 * Configuration webserver
 */
WebServer server(&settings);

//const char* deviceName = "feedback-1";

/** Wifi configuration */
const char* ssid = "Tanemahuta";
const char* password = "7ykcRytmjs6k";

/*
const char* ssid = "Namics";
const char* password = "NamicsSeit1995";
*/

enum State {
  FEEDBACK, FEEDBACK_ERROR, SERVER
};

State currentState = FEEDBACK;

void startConfigurationServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Creating AP ");
    Serial.print(settings.serverSettings.deviceName);
    Serial.print(" with password '");
    Serial.print(settings.serverSettings.password);
    Serial.println("'");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(settings.serverSettings.deviceName, settings.serverSettings.password);
    server.start(WiFi.softAPIP());
    ledError.blink(200);
  } else {
    server.start(WiFi.localIP());
  }
}

/**
   Wifi connection method.
*/
void connectToWifi() {
  Serial.print("Connecting to ");
  Serial.print(settings.wifiSettings.ssid);
  Serial.print("...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.wifiSettings.ssid, settings.wifiSettings.password);
  long start = millis();
  while (WiFi.status() != WL_CONNECTED &&
         WiFi.status() != WL_CONNECT_FAILED &&
         (millis()-start) < WIFI_TIMEOUT_MILLIS) {
    delay(50);
    ledReady.handle();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("connected.");
    ledReady.on();
    currentState = FEEDBACK;
  } else {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.print("failed after ");
    } else {
      Serial.print("timed out after ");
    }
    Serial.print((millis()-start));
    Serial.println("ms.");
  }
  startConfigurationServer();
}

void setupGforms() {
  gForms.setup(settings.gformsSettings.formUrl, settings.gformsSettings.entry);
}

void doSetup() {
  Serial.println("Setting up device.");
  ledError.off();
  ledReady.blink(500);
  connectToWifi();
  if (currentState == FEEDBACK) {
    setupGforms();
    ledError.off();
    ledReady.on();
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  settings.load();
  doSetup();
}

bool isServerSetupComboPressed() {
  return switchHandler.getPressedCount() == 2 &&
         switchHandler.isPressed(0) &&
         switchHandler.isPressed(switchHandler.getSwitchesCount() - 1);
}

void handleFeedback() {
  switchHandler.refresh();
  if (isServerSetupComboPressed()) {
    Serial.println("Server setup combo pressed, enabling gforms configuration");
    server.enableGforms();
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
        ledReady.off();
        Serial.print("Found feedback button ");
        Serial.print(i);
        Serial.println(" to be released, sending feedback.");
        if (gForms.sendFeedback(i)) {
          ledReady.blinkBlocking(250, 2);
          ledReady.on();
        } else {
          ledReady.off();          
          ledError.blinkBlocking(100, 5);
          ledReady.on();          
        }
      }
    }
  }
  delay(10);
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

void loop() {
  switch (currentState) {
    case FEEDBACK: handleFeedback(); break;
    case FEEDBACK_ERROR: handleFeedbackError(); break;
    case SERVER:
      if (server.isDone()) {
        Serial.println("Switched back to feedback mode.");
        setupGforms();
        currentState = FEEDBACK_ERROR;
        return;
      }
    break;
  }
  ledReady.handle();
  ledError.handle();
  server.handleClient();           
  delay(10);
}

