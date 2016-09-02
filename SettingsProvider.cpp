#include "SettingsProvider.h"

SettingsProvider::SettingsProvider() : 
  wifiSettings({ new char[WIFI_SSID_LENGTH], new char[WIFI_PASSWORD_LENGTH]}), 
  gformsSettings({new char[GFORMS_URL_LENGTH], new char[GFORMS_ENTRY_LENGTH]}),
  serverSettings({ new char[WIFI_SSID_LENGTH], new char[WIFI_PASSWORD_LENGTH]}) {
  setDefaultServerSettings();
  EEPROM.begin(1024);
 }

SettingsProvider::~SettingsProvider() {
  delete wifiSettings.ssid;
  delete wifiSettings.password;
  delete serverSettings.deviceName;
  delete serverSettings.password;
  delete gformsSettings.formUrl;
  delete gformsSettings.entry;
}

int readString(int base, int size, char* target) {
  String result;
  Serial.print("Read from ");
  Serial.print(base);
  Serial.print(": ");
  for (int i = 0; i < size; i++) {
    char c = char(EEPROM.read(base+i));
    result += c;
    Serial.print(c);
  }
  Serial.print(" -> ");
  result.toCharArray(target, size);
  Serial.println(target);
  return base+size;
}

int writeString(int base, char* source, int max) {
  String string = String(source);
  Serial.print("Write to ");
  Serial.print(base);
  Serial.print(": ");
  Serial.print(source);
  Serial.print(" -> ");
  for (int i = 0; i < string.length(); i++) {
    Serial.print(string[i]);
    EEPROM.write(i+base, string[i]);
  }
  Serial.println();
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
    Serial.print("Set default deviceName=");
    Serial.print(deviceName);
    Serial.print(" password=");
    Serial.println(DEFAULT_SERVER_PASSWORD);
    deviceName.toCharArray(serverSettings.deviceName, WIFI_SSID_LENGTH);
    DEFAULT_SERVER_PASSWORD.toCharArray(serverSettings.password, WIFI_PASSWORD_LENGTH);
  }
}

void SettingsProvider::load() {
  int addr = 0;
  addr = readString(addr, WIFI_SSID_LENGTH, serverSettings.deviceName);
  addr = readString(addr, WIFI_PASSWORD_LENGTH, serverSettings.password);
  addr = readString(addr, WIFI_SSID_LENGTH, wifiSettings.ssid);
  addr = readString(addr, WIFI_PASSWORD_LENGTH, wifiSettings.password);
  addr = readString(addr, GFORMS_URL_LENGTH, gformsSettings.formUrl);
  addr = readString(addr, GFORMS_ENTRY_LENGTH, gformsSettings.entry);
  setDefaultServerSettings();
  Serial.println("Loaded settings from EEPROM");
}

void SettingsProvider::save() {
  int addr = 0;
  addr = writeString(addr, serverSettings.deviceName, WIFI_SSID_LENGTH);
  addr = writeString(addr, serverSettings.password, WIFI_PASSWORD_LENGTH);
  addr = writeString(addr, wifiSettings.ssid, WIFI_SSID_LENGTH);
  addr = writeString(addr, wifiSettings.password, WIFI_PASSWORD_LENGTH);
  addr = writeString(addr, gformsSettings.formUrl, GFORMS_URL_LENGTH);
  addr = writeString(addr, gformsSettings.entry, GFORMS_ENTRY_LENGTH);
  if (EEPROM.commit()) {
    Serial.println("Saved settings to EEPROM");
  } else {
    Serial.println("Failed to commit EEPROM ");
  }
}

