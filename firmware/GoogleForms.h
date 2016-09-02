#ifndef GOOGLE_FORMS_H
#define GOOGLE_FORMS_H
 
#include <Arduino.h>
#include <WiFiClientSecure.h>

class GoogleForms {
  public:
        GoogleForms();
        ~GoogleForms();
        boolean setup(String url, String entry);
        boolean sendFeedback(int feedback);
  private:
        char* _host;
        char* _path;
        char* _entry;
        WiFiClientSecure* _client;
        boolean findEntry();

};

#endif
