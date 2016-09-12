#ifndef SETTINGS_PROVIDER_H
#define SETTINGS_PROVIDER_H

#define MAX_WIFI_CONNECTION_COUNT 1
#define WIFI_SSID_LENGTH 64
#define WIFI_PASSWORD_LENGTH 64
#define GFORMS_URL_LENGTH 128

#define SETTINGS_VERSION 1
#define EEPROM_SIZE MAX_WIFI_CONNECTION_COUNT*(WIFI_SSID_LENGTH + WIFI_PASSWORD_LENGTH) + GFORMS_URL_LENGTH

#define DEFAULT_SERVER_PASSWORD String("feedback")

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

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
};

class SettingsProvider {
  public:
    SettingsProvider();
    ~SettingsProvider();
    bool load();
    void save();
    const WifiSettings wifiSettings;
    const GformsSettings gformsSettings;
    const ServerSettings serverSettings;
  private:
    void setDefaultServerSettings();
};

#endif
