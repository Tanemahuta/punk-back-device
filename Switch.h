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
    int pin_;
    long lastReadHigh_;
    long lastReadLow_;
    int state_;
    bool changed_;
    bool released_;
};


#endif
