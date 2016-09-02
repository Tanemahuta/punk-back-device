#ifndef SETTINGS_PROVIDER_H
#define SETTINGS_PROVIDER_H

#define WIFI_SSID_LENGTH 64
#define WIFI_PASSWORD_LENGTH 64
#define GFORMS_URL_LENGTH 128
#define GFORMS_ENTRY_LENGTH 128
#define DEFAULT_SERVER_PASSWORD String("feedback")

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Arduino.h>

struct WifiSettings {
  char* ssid;
  char* password;
};

struct ServerSettings {
  char* deviceName;
  char* password;
};

struct GformsSettings {
  char* formUrl;
  char* entry;
};

class SettingsProvider {
  public:
    SettingsProvider();
    ~SettingsProvider();
    void load();
    void save();
    const WifiSettings wifiSettings;
    const GformsSettings gformsSettings;
    const ServerSettings serverSettings;
  private:
    void setDefaultServerSettings();
};

#endif
