#include "GoogleForms.h"

#define LE "\r\n"
#define LINE(str) str+LE
#define POST(url) LINE(String("POST ") + url + " HTTP/1.1")
#define HEADER(id, value) LINE(id + ": " + value)

GoogleForms::GoogleForms(WiFiClientSecure client) {
  _client = client;
}
GoogleForms::~GoogleForms() {
  // NOP
}
       
boolean GoogleForms::setup(String url, String entry) {
  return true;
}

boolean GoogleForms::sendFeedback(int value) {

  if (_path == NULL || _entry == NULL) {
    Serial.println("Google form not setup!");
    return false;
  }
  
  Serial.print("Connecting to ");
  Serial.print(_host);
  Serial.print("...");
  if (!_client.connect(_host, 443)) {
    Serial.println("FAILED!");
    return false;
  }
  
  String payload = String(_entry) + "=" + String(value);
  String path = String(_path) + "/formResponse";
  String request = POST(path) +
                   HEADER("Host", _host) +
                   HEADER("User-Agent", "Mozilla/4.0 (compatible; F-Device 1.0)") +
                   HEADER("Connection", "close") +
                   HEADER("Content-Type", "application/x-www-form-urlencoded") +
                   HEADER("Content-Length", String(payload.length())) + LE +
                   payload;
  
  Serial.println("Sending request:");
  Serial.println(request);
  _client.print(request);
  Serial.println("Request sent.");

  String response = _client.readStringUntil('\n');
  bool ok = response.indexOf("HTTP/1.1 200 OK") == 0;

  if (ok) {
    Serial.println("Received 200 response.");
    return true;
  } else {
    Serial.print("Received errornous response: ");
    Serial.println(response);
    return false;
  }
}
