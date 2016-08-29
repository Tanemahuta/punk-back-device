#include "SwitchHandler.h"

SwitchHandler::SwitchHandler(const Switch switches[], int count) {
  _switches = switches;
  _switchesCount = count;
  refresh();
}

SwitchHandler::~SwitchHandler() {
}

int SwitchHandler::getSwitchesCount() {
  return _switchesCount;
}

void SwitchHandler::refresh() {
  _pressedCount = 0;
  _wasChanged = false;
  for (int i = 0; i < _switchesCount; i++) {
    Switch sw = _switches[i];
    sw.refresh();
    if (sw.getState()) {
      _pressedCount++;
    }
    _wasChanged |= sw.wasChanged();
  }
}

int SwitchHandler::getPressedCount() {
  return _pressedCount;
}

bool SwitchHandler::isPressed(int idx) {
  if (idx < _switchesCount) {
    Switch sw = _switches[idx];
    return sw.getState();
  }
  return false;
}

bool SwitchHandler::wasReleased(int idx) {
  if (idx < _switchesCount) {
    Switch sw = _switches[idx];
    return sw.wasReleased();
  }
  return false;
}


bool SwitchHandler::wasChanged() {
  return _wasChanged;
}


