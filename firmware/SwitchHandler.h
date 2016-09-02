#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H

#define SWITCHES_COUNT 4

#include <Arduino.h>
#include "Switch.h"

class SwitchHandler {
    public:
      SwitchHandler(int pin1, int pin2, int pin3, int pin4);
      ~SwitchHandler();
      void refresh();
      int getPressedCount();
      int getSwitchesCount();
      bool isPressed(int idx);
      bool wasReleased(int idx);
      bool wasChanged();
    private:
      Switch** _switches;
      int _pressedCount;
      bool _wasChanged;
};

#endif
