#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "SettingsProvider.h"

class WebServer {
  public:
    WebServer(SettingsProvider* settingsProvider);
    ~WebServer();
    void start(IPAddress ip);
    void handleClient();
    bool isDone();
    void rootPageHandler();
    void devicePageHandler();
    void wifiPageHandler();
    void gformsPageHandler();
    void feedbackModeHandler();
    void cssHandler();
    void enableGforms();
    void handleNotFound();
  private:
    bool _done;
    bool _enableGforms;
    ESP8266WebServer* _server;
    SettingsProvider* _settingsProvider;    
};

#endif
