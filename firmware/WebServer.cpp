#include "WebServer.h"

#include "HTML_CSS.h"
#include "Logging.h"

#define CALL_STRUCT(methodName) \
  struct Call_##methodName {\
    WebServer & _src;\
    Call_##methodName(WebServer & src) : _src(src) {\
    }\
    void operator()() const {\
      _src.methodName();\
    }\
  }

#define PATH_DEVICE_CFG "/device_config"
#define PATH_WIFI_CFG "/wifi_config"
#define PATH_GFORMS_CFG "/gforms_config"
#define PATH_SWITCH_MODE "/feedbackMode"

CALL_STRUCT(rootPageHandler);
CALL_STRUCT(wifiPageHandler);
CALL_STRUCT(gformsPageHandler);
CALL_STRUCT(handleNotFound);
CALL_STRUCT(devicePageHandler);
CALL_STRUCT(feedbackModeHandler);
CALL_STRUCT(cssHandler);

WebServer::WebServer(SettingsProvider* settingsProvider): _settingsProvider(settingsProvider), _done(false), _enableGforms(false) {
  _server = new ESP8266WebServer(80);
  _server->on("/", Call_rootPageHandler(*this));
  _server->on(PATH_DEVICE_CFG, Call_devicePageHandler(*this));
  _server->on(PATH_WIFI_CFG, Call_wifiPageHandler(*this));
  _server->on(PATH_GFORMS_CFG, Call_gformsPageHandler(*this));
  _server->on(PATH_SWITCH_MODE, Call_feedbackModeHandler(*this));
  _server->on(PATH_CSS, Call_cssHandler(*this));
  _server->onNotFound(Call_handleNotFound(*this));
}

void WebServer::enableGforms() {
  _enableGforms = true;
}

void WebServer::disableGforms() {
  _enableGforms = false;
  _done = true;
}

WebServer::~WebServer() {
  delete _server;
}
void WebServer::start(IPAddress ip) {
  if (!MDNS.begin(_settingsProvider->serverSettings.deviceName, ip)) {
    Log_Error(String("Could not start mDNS server: ")+_settingsProvider->serverSettings.deviceName);
  } else {
    MDNS.addService("http", "tcp", 80);
  }
  _done = false;
  _server->begin();
}

void WebServer::handleClient() {
  _server->handleClient();
}

bool WebServer::isDone() {
  return _done;
}

void appendHeader(String &response, char* title) {
  response += HTML_HEAD;
  response += HTML_BODY;
  response += BODY_TITLE_START;
  response += title;
  response += BODY_TITLE_END;  response += "<center>WIFI Status: ";
  if (WiFi.status() == WL_CONNECTED) {
    response += "Connected to ";
    response += WiFi.SSID();
  } else {
    response += "Disconnected.";
  }
  response += "</center>";
}

void appendFooter(String &response) {
  response += "</body></html>";
}

void appendLink(String &response, char* href, char* text) {
  response += "<a href=\"";
  response += href;
  response += "\">";
  response += text;
  response += "</a>";
}

void appendInput(String &response, String type, String name, String id, String value, String text = "") {

  response += "<input type=\"";
  response += type;
  response += "\" name=\"";
  response += name;
  response += "\" id=\"";
  response += name;
  if (value.length() > 0) {
    response += "\" value=\"";
    response += value;
  }
  if (text.length() > 0) {
    response += "\" text=\"";
    response += text;  
  }
  response += "\" />";
}

void appendLabel(String &response, String name, String label) {
  response += "<label for=\"";
  response += name;
  response += "\">";
  response += label;
  response += "</label>";
}

void redirectToStart(ESP8266WebServer* _server, String msg) {
  String location = "/";
  if (msg.length() > 0) {
    location += "?msg=";
    location += msg;
  }
  _server->sendHeader("Location", location , true);
  _server->send( 302, "text/plain", "");
}

/* Root page for the webserver */
void WebServer::rootPageHandler()
{
  String response = "";  
  appendHeader(response, "F-Device status");  
  if (_server->hasArg("msg")) {
    response += "<div class=\"message\">";
    response += _server->arg("msg");
    response += "</div>";
  }
  response += "<div class=\"linkCollection\">";
  appendLink(response, PATH_DEVICE_CFG, "Configure device");
  response += "<br/>";
  appendLink(response, PATH_WIFI_CFG, "Configure WiFi connection");
  response += "<br/>";
  if (_enableGforms) {
    appendLink(response, PATH_GFORMS_CFG, "Configure google form");
    response += "<br/>";
    appendLink(response, PATH_SWITCH_MODE, "Restart to feedback mode");
    response += "<br/>";
  }
  response += "</div>";
  appendFooter(response);

  _server->send(200, "text/html", response);
}

void WebServer::devicePageHandler() {
  String response = "";
  appendHeader(response, "Device configuration");
  if (_server->hasArg("deviceName")) {
    String ssid = _server->arg("deviceName");
    ssid.toCharArray(_settingsProvider->serverSettings.deviceName, 64);
    String pass = _server->arg("password");
    pass.toCharArray(_settingsProvider->serverSettings.password, 64);
    _settingsProvider->save();
    redirectToStart(_server, "Device settings saved.");
    return;
  }

  response += "<form method=\"post\">";
  appendLabel(response, "deviceName", "Device name (fallback SSID)");
  appendInput(response, "text", "deviceName", "deviceName", _settingsProvider->serverSettings.deviceName);
  response += "<br />";
  appendLabel(response, "password", "Device password (fallback SSID)");
  appendInput(response, "password", "password", "password", _settingsProvider->serverSettings.password);
  response += "<br />";
  appendInput(response, "button\" onclick=\"location.href = '/'\"", "back", "back", "Back");
  appendInput(response, "submit", "save", "save", "Save");
  response += "</form>";
  appendFooter(response);
  _server->send(200, "text/html", response);
}

void WebServer::feedbackModeHandler() {
  String response = "";
  _enableGforms = false;
  redirectToStart(_server, "Restarted Feedback process.");
  _done = true;  
}

/* WLAN page allows users to set the WiFi credentials */
void WebServer::wifiPageHandler()
{
  // Check if there are any GET parameters
  if (_server->hasArg("ssid")) {
    String ssid = _server->arg("ssid");
    ssid.toCharArray(_settingsProvider->wifiSettings.ssid, 64);
    Log_Info(String("Configured WiFi for ssid ")+ssid);
    String pass = _server->arg("password");
    pass.toCharArray(_settingsProvider->wifiSettings.password, 64);
    _settingsProvider->save();
    redirectToStart(_server, "WiFi settings saved.");
    return;
  }

  String response = "";
  appendHeader(response, "WiFi configuration");
  
  response += "<p>To connect to a WiFi network, please select a network...</p>";

  // Get number of visible access points
  int ap_count = WiFi.scanNetworks();

  if (ap_count == 0) {
    response += "No access points found.<br>";
  } else {
    response += "<form method=\"post\">";

    // Show access points
    for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
    {
      String id(WiFi.SSID(ap_idx));
      String label(WiFi.SSID(ap_idx));
      if (WiFi.encryptionType(ap_idx) != ENC_TYPE_NONE) {
        label += "*";
      }
      appendInput(response, "radio", "ssid", id, id);
      appendLabel(response, id, label);
      response += "<br />";
    }
    appendLabel(response, "password", "WiFi password (if required):");
    appendInput(response, "password", "password", "password", _settingsProvider->serverSettings.password);
    response += "<br />";
    appendInput(response, "button\" onclick=\"location.href = '/'\"", "back", "back", "Back");
    appendInput(response, "submit", "save", "save", "Save");
    response += "</form>";
  }

  appendFooter(response);
  _server->send(200, "text/html", response);
}

/* GPIO page allows you to control the GPIO pins */
void WebServer::gformsPageHandler()
{
  if (!_enableGforms) {
    redirectToStart(_server, "Changing feedback not enabled.");
    return;
  }
  // Check if there are any GET parameters
  if (_server->hasArg("formUrl")) {
    String ssid = _server->arg("formUrl");
    ssid.toCharArray(_settingsProvider->gformsSettings.formUrl, 128);
    _settingsProvider->save();
    redirectToStart(_server, "Google forms settings saved.");
    return;
  }

  String response = "";
  appendHeader(response, "Google forms configuration");
  response += "<form method=\"post\">";
  appendLabel(response, "formsUrl", "Google forms URL:");
  appendInput(response, "text", "formUrl", "formUrl", _settingsProvider->gformsSettings.formUrl);
  response += "<br />";
  appendInput(response, "button\" onclick=\"location.href = '/'\"", "back", "back", "Back");
  appendInput(response, "submit", "save", "save", "Save");
  response += "</form>";
  appendFooter(response);
  _server->send(200, "text/html", response);
}

void WebServer::cssHandler() {
  _server->send(200, "text/css", CSS_CONTENT);
}

/* Called if requested page is not found */
void WebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += _server->uri();
  message += "\nMethod: ";
  message += (_server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += _server->args();
  message += "\n";

  for (uint8_t i = 0; i < _server->args(); i++)
  {
    message += " " + _server->argName(i) + ": " + _server->arg(i) + "\n";
  }

  _server->send(404, "text/plain", message);
}

