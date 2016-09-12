#include "GoogleForms.h"

#include "Logging.h"

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
       
boolean GoogleForms::setup(String url) {
  int curIndex = url.indexOf("https://");
  if (curIndex != 0) {
    Log_Error(String("Url does not start with https://: ")+url);
    return false;
  }
  curIndex = url.indexOf("/", 8);
  if (curIndex <9) {
    Log_Error(String("Url does not contain a valid path: ")+url);
    return false;    
  }
  url.substring(8, curIndex).toCharArray(_host, 128);
  Log_Debug(String("Using forms host: ")+_host);
  int lastIndex = url.indexOf("/viewform", curIndex);
  if (lastIndex < curIndex) {
    lastIndex -= url.length();
  }
  url.substring(curIndex, lastIndex).toCharArray(_path, 128);
  Log_Debug(String("Setup Gforms path: ")+_path);
  if (!findEntry()) {
    Log_Error(String("Could not find entry for the provided url: ")+url);
  }  
  Log_Info(String("Found gform entry: ")+_entry);
  return true;
}

boolean GoogleForms::findEntry() {
    if (_host == NULL || _path == NULL) {
    Log_Error(String("Google form not setup!"));
    return false;
  }
  
  Log_Debug(String("Connecting to ")+_host+" ...");
  if (!_client->connect(_host, 443)) {
    Log_Error(String("Connection to ")+_host+"failed!");
    return false;
  }
  
  String path = String(_path) + "/viewform";
  String request = GET(path) +
                   HEADER("Host", _host) +
                   HEADER("User-Agent", "Mozilla/4.0 (compatible; F-Device 1.0)") +
                   HEADER("Connection", "close") +
                   HEADER("Accept-Type", "text/html") + LE;
  
  Log_Debug(String("Sending request: ")+request);
  _client->print(request);
  Log_Info(String("Google form 'viewform' request sent."));

  String response = _client->readStringUntil('\n');
  bool ok = response.indexOf("HTTP/1.1 200 OK") == 0;

  if (ok) {
    Log_Info(String("Received 200 response."));
    String entryName = "";
    do {
      String tmp = _client->readStringUntil('"');
      if (tmp.startsWith("entry")) {
        entryName += tmp;
        break;
      }
    } while (_client->connected());
    if (entryName.length() == 0) {
      Log_Error(String("Could not find first entry field for the form: ")+_path);
      _client->stop();
      return false;
    }
    Log_Info(String("Found entry for the form ")+_path+" :"+entryName);
    entryName.toCharArray(_entry, 128);
    _client->stop();
    return true;
  } else {
    Log_Error(String("Received a non 200 response:")+response);
    _client->stop();
    return false;
  }
}

boolean GoogleForms::sendFeedback(int value) {

  if (_host == NULL || _path == NULL || _entry == NULL) {
    Log_Error(String("Google form is not setup."));
    return false;
  }
  Log_Debug(String("Connecting to ")+_host+" ...");
  if (!_client->connect(_host, 443)) {
    Log_Error(String("Could not connect to host ")+_host);
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
  
  Log_Debug(String("Sending request ")+request);
  _client->print(request);
  Log_Info(String("Google form 'formResponse' request sent."));

  String response = _client->readStringUntil('\n');
  bool ok = response.indexOf("HTTP/1.1 200 OK") == 0;

  if (ok) {
    Log_Info(String("Received 200 response."));
    _client->stop();
    return true;
  } else {
    Log_Info(String("Received a non 200 response: ")+response);
    _client->stop();
    return false;
  }
}
