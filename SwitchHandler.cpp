#include "SwitchHandler.h"

SwitchHandler::SwitchHandler(int pin1, int pin2, int pin3, int pin4) {
  _switches = new Switch*[4];
  _switches[0] = new Switch(pin1);
  _switches[1] = new Switch(pin2);
  _switches[2] = new Switch(pin3);
  _switches[3] = new Switch(pin4);
  refresh();
}

SwitchHandler::~SwitchHandler() {
  delete _switches[0];
  delete _switches[1];
  delete _switches[2];
  delete _switches[3];
  delete _switches;
}

int SwitchHandler::getSwitchesCount() {
  return SWITCHES_COUNT;
}

void SwitchHandler::refresh() {
  _pressedCount = 0;
  _wasChanged = false;
  for (int i = 0; i < SWITCHES_COUNT; i++) {
    Switch* sw = _switches[i];
    sw->refresh();
    if (sw->getState()) {
      _pressedCount++;
    }
    _wasChanged |= sw->wasChanged();
  }
}

int SwitchHandler::getPressedCount() {
  return _pressedCount;
}

bool SwitchHandler::isPressed(int idx) {
  if (idx < SWITCHES_COUNT) {
    Switch* sw = _switches[idx];
    return sw->getState();
  }
  return false;
}

bool SwitchHandler::wasReleased(int idx) {
  if (idx < SWITCHES_COUNT) {
    Switch* sw = _switches[idx];
    return sw->wasReleased();
  }
  return false;
}


bool SwitchHandler::wasChanged() {
  return _wasChanged;
}


