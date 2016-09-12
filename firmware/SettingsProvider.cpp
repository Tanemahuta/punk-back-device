#include "SettingsProvider.h"

#include "Logging.h"

SettingsProvider::SettingsProvider() : 
  wifiSettings({ new char[WIFI_SSID_LENGTH], new char[WIFI_PASSWORD_LENGTH]}), 
  gformsSettings({new char[GFORMS_URL_LENGTH]}),
  serverSettings({ new char[WIFI_SSID_LENGTH], new char[WIFI_PASSWORD_LENGTH]}) {
  setDefaultServerSettings();
  EEPROM.begin(EEPROM_SIZE);
 }

SettingsProvider::~SettingsProvider() {
  delete wifiSettings.ssid;
  delete wifiSettings.password;
  delete serverSettings.deviceName;
  delete serverSettings.password;
  delete gformsSettings.formUrl;
}

int readString(int base, int size, char* target) {
  String result;
  Log_Debug(String("Reading from ")+base);
  for (int i = 0; i < size; i++) {
    char c = char(EEPROM.read(base+i));
    result += c;
  }
  Log_Debug(String("Read ")+result);
  result.toCharArray(target, size);
  return base+size;
}

int writeString(int base, char* source, int max) {
  String string = String(source);
  Log_Debug(String("Writing to ")+base);
  for (int i = 0; i < string.length(); i++) {
    EEPROM.write(i+base, string[i]);
  }
  Log_Debug(String("Wrote ")+source);
  for (int i = string.length(); i < max; i++) {
    EEPROM.write(i+base, 0);
  }
  return base+max;
}

void SettingsProvider::setDefaultServerSettings() {
  String deviceName(serverSettings.deviceName);
  if (deviceName.length() == 0) {    
    byte mac[6];
    WiFi.macAddress(mac);
    deviceName += "feedback-";
    for (int i = 0; i < 6; i++) {
      deviceName += mac[i];
    }
    Log_Debug(String("Set default device name ")+deviceName+" and password "+DEFAULT_SERVER_PASSWORD);
    deviceName.toCharArray(serverSettings.deviceName, WIFI_SSID_LENGTH);
    DEFAULT_SERVER_PASSWORD.toCharArray(serverSettings.password, WIFI_PASSWORD_LENGTH);
  }
}

bool SettingsProvider::load() {
  int version = EEPROM.read(0);
  bool result;
  if (version != SETTINGS_VERSION) {
    Log_Error(String("Settings are not in the correct format"));
    String("").toCharArray(serverSettings.deviceName, WIFI_SSID_LENGTH);
    String("").toCharArray(serverSettings.password, WIFI_PASSWORD_LENGTH);
    result = false;
  } else {
    Log_Debug(String("Read version eeprom settings version ")+version);
    int addr = sizeof(version);
    addr = readString(addr, WIFI_SSID_LENGTH, serverSettings.deviceName);
    addr = readString(addr, WIFI_PASSWORD_LENGTH, serverSettings.password);
    addr = readString(addr, WIFI_SSID_LENGTH, wifiSettings.ssid);
    addr = readString(addr, WIFI_PASSWORD_LENGTH, wifiSettings.password);
    addr = readString(addr, GFORMS_URL_LENGTH, gformsSettings.formUrl);
    Log_Info(String("Loaded settings from EEPROM"));
    result = true;
  }
  setDefaultServerSettings();
  return result;
}

void SettingsProvider::save() {
  int settingsVersion = SETTINGS_VERSION;
  EEPROM.write(0, settingsVersion);
  int addr = sizeof(settingsVersion);
  addr = writeString(addr, serverSettings.deviceName, WIFI_SSID_LENGTH);
  addr = writeString(addr, serverSettings.password, WIFI_PASSWORD_LENGTH);
  addr = writeString(addr, wifiSettings.ssid, WIFI_SSID_LENGTH);
  addr = writeString(addr, wifiSettings.password, WIFI_PASSWORD_LENGTH);
  addr = writeString(addr, gformsSettings.formUrl, GFORMS_URL_LENGTH);
  if (!EEPROM.commit()) {
    Log_Error(String("Failed to commit EEPROM!"));
    return;
  }
  Log_Info(String("Saved settings to EEPROM."));
}

