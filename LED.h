#ifndef LED_H
#define LED_H
 
#include <Arduino.h>
 
class LED {
  public:
        LED(int pin);
        ~LED();
        bool getState();
        void on();
        void off();
        void negate();
        void blink(int time);
        void blinkBlocking(int time, int times);
        void handle();
  private:
        int _pin;
        bool _state;
        long _lastHandled;
        int _blinkingTime;
        void setState(bool state);        
};
 
#endif
