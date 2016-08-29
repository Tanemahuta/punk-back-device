#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H

#include <Arduino.h>
#include "Switch.h"

class SwitchHandler {
    public:
      SwitchHandler(const Switch switches[], int count);
      ~SwitchHandler();
      void refresh();
      int getPressedCount();
      int getSwitchesCount();
      bool isPressed(int idx);
      bool wasReleased(int idx);
      bool wasChanged();
    private:
      const Switch* _switches;
      int _switchesCount;
      int _pressedCount;
      bool _wasChanged;
};

#endif
