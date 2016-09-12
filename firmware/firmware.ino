#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "LED.h"
#include "SwitchHandler.h"
#include "GoogleForms.h"
#include "WebServer.h"
#include "SettingsProvider.h"
#include "Logging.h"

#define WIFI_TIMEOUT_MILLIS 30*1000

/** LEDs */
LED ledReady(D0);
LED ledError(D1);

/** Switches */
SwitchHandler switchHandler(D4, D5, D6, D7);

/** Google forms handler */
GoogleForms gForms;

/** Settings provider for persistent settings */
SettingsProvider settings;

/**
   Configuration webserver
*/
WebServer server(&settings);

enum State {
  FEEDBACK, FEEDBACK_ERROR, SERVER
};

State currentState = SERVER;

void setupSoftAP() {
  Log_Info(String("Creating AP ") + settings.serverSettings.deviceName + " width password " + settings.serverSettings.password);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(settings.serverSettings.deviceName, settings.serverSettings.password);
  ledReady.off();
  ledError.blink(200);
}

void startConfigurationServer(IPAddress ip) {
  Log_Info(String("Created configuration server."));
  server.start(ip);
}

/**
   Wifi connection method.
*/
bool connectToWifi() {
  Log_Info(String("Connecting to ")+settings.wifiSettings.ssid+" ...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.wifiSettings.ssid, settings.wifiSettings.password);
  long start = millis();
  while (WiFi.status() != WL_CONNECTED &&
         WiFi.status() != WL_CONNECT_FAILED &&
         (millis() - start) < WIFI_TIMEOUT_MILLIS) {
    delay(50);
    ledReady.handle();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Log_Info(String("Connected to WiFi."));
    ledReady.on();
    return true;    
  } else {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Log_Error(String("Could not connect to WiFi."));
    } else {
      Log_Error(String("Connection to WiFi timed out."));
    }
    return false;
  }
}

void setupGforms() {
  ledReady.off();
  gForms.setup(settings.gformsSettings.formUrl);
  ledReady.on();
  ledError.off();
}

void setup() {
  Log.Init(LOG_LEVEL_DEBUG, 115200);
  delay(10);
  if (!settings.load()) {
    Log_Error(String("Could not load settings, switching to configuration mode."));
    setupSoftAP();
    startConfigurationServer(WiFi.softAPIP());
  } else {

    Log_Info(String("Setting up feedback device."));
    ledError.off();
    ledReady.blink(500);
    IPAddress ip;
    if (connectToWifi()) {
      setupGforms();
      ledError.off();
      ledReady.on();
      currentState = FEEDBACK;
      ip = WiFi.localIP();
    } else {
      ledError.blink(200);
      ledReady.off();
      Log_Error(String("Could not connect to WiFi, setting up soft AP."));
      setupSoftAP();
      ip = WiFi.softAPIP();
    }
    startConfigurationServer(ip);
  }
}

bool isServerSetupComboPressed() {
  return switchHandler.getPressedCount() == 2 &&
         switchHandler.isPressed(0) &&
         switchHandler.isPressed(switchHandler.getSwitchesCount() - 1);
}

void handleFeedback() {
  switchHandler.refresh();
  if (isServerSetupComboPressed()) {
    Log_Info(String("Server setup combo pressed, enabling gforms configuration"));
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
      switchHandler.refresh();
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

