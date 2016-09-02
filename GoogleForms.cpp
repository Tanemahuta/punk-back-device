#include "GoogleForms.h"

#define LE "\r\n"
#define LINE(str) str+LE
#define POST(url) LINE(String("POST ") + url + " HTTP/1.1")
#define GET(url) LINE(String("GET ") + url + " HTTP/1.1")
#define HEADER(id, value) LINE(id + ": " + value)

GoogleForms::GoogleForms() : _host(new char[128]), _path(new char[128]), _entry(new char[128]) {
  _client = new WiFiClientSecure();
}
GoogleForms::~GoogleForms() {
  delete _client;
}
       
boolean GoogleForms::setup(String url, String entry) {
  int curIndex = url.indexOf("https://");
  if (curIndex != 0) {
    Serial.print("Url does not start with https:// : ");
    Serial.println(url);
    return false;
  }
  curIndex = url.indexOf("/", 8);
  if (curIndex <9) {
    Serial.print("Url does not contain a valid path: ");
    Serial.println(url);
    return false;    
  }
  url.substring(8, curIndex).toCharArray(_host, 128);
  Serial.print("Setup Gforms host: ");
  Serial.println(_host);
  int lastIndex = url.indexOf("/viewform", curIndex);
  if (lastIndex < curIndex) {
    lastIndex -= url.length();
  }
  url.substring(curIndex, lastIndex).toCharArray(_path, 128);
  Serial.print("Setup Gforms path: ");
  Serial.println(_path);
  if (!findEntry()) {
    entry.toCharArray(_entry, 128);
  }  
  Serial.print("Setup Gforms entry: ");
  Serial.println(_entry);
  return true;
}

boolean GoogleForms::findEntry() {
    if (_host == NULL || _path == NULL) {
    Serial.println("Google form not setup!");
    return false;
  }
  
  Serial.print("Connecting to ");
  Serial.print(_host);
  Serial.print("...");
  if (!_client->connect(_host, 443)) {
    Serial.println("FAILED!");
    return false;
  }
  
  String path = String(_path) + "/viewform";
  String request = GET(path) +
                   HEADER("Host", _host) +
                   HEADER("User-Agent", "Mozilla/4.0 (compatible; F-Device 1.0)") +
                   HEADER("Connection", "close") +
                   HEADER("Accept-Type", "text/html") + LE;
  
  Serial.println("Sending request:");
  Serial.println(request);
  _client->print(request);
  Serial.println("Request sent.");

  String response = _client->readStringUntil('\n');
  bool ok = response.indexOf("HTTP/1.1 200 OK") == 0;

  if (ok) {
    Serial.println("Received 200 response.");
    String entryName = "";
    do {
      String tmp = _client->readStringUntil('"');
      if (tmp.startsWith("entry")) {
        entryName += tmp;
        break;
      }
    } while (_client->connected());
    if (entryName.length() == 0) {
      Serial.println();
      Serial.println("Could not find input entry.");
      _client->stop();
      return false;
    }
    Serial.println();
    Serial.print("Found entry: ");
    Serial.println(entryName);
    entryName.toCharArray(_entry, 128);
    _client->stop();
    return true;
  } else {
    Serial.print("Received errornous response: ");
    Serial.println(response);
    _client->stop();
    return false;
  }
}

boolean GoogleForms::sendFeedback(int value) {

  if (_host == NULL || _path == NULL || _entry == NULL) {
    Serial.println("Google form not setup!");
    return false;
  }
  
  Serial.print("Connecting to ");
  Serial.print(_host);
  Serial.print("...");
  if (!_client->connect(_host, 443)) {
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
  _client->print(request);
  Serial.println("Request sent.");

  String response = _client->readStringUntil('\n');
  bool ok = response.indexOf("HTTP/1.1 200 OK") == 0;

  if (ok) {
    Serial.println("Received 200 response.");
    _client->stop();
    return true;
  } else {
    Serial.print("Received errornous response: ");
    Serial.println(response);
    _client->stop();
    return false;
  }
}
