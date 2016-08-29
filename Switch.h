#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>

class Switch {
  public:
    Switch(int pin);
    ~Switch();
    void refresh();
    bool wasChanged();
    bool getState();
    bool wasReleased();
  private:
    int _pin;
    bool _state;
    bool _changed;
    bool _released;
};


#endif
