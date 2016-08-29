#include "Switch.h"

Switch::Switch(int pin) {
  _pin = pin;
  pinMode(pin, INPUT_PULLUP);
}

Switch::~Switch() {
  // Nothing to do  
}

void Switch::refresh() {
  bool state = !digitalRead(_pin);
  if (_state != state) {
    _changed = true;
    _released = _state && !state;
    Serial.println(state ? "Button pressed." : "Button released.");
  } else {
    _released = false;
    _changed = false;
  }
  _state = state;
}

bool Switch::wasReleased() {
  return _released;
}

bool Switch::wasChanged() {
  return _changed;
}

bool Switch::getState() {
  return _state;
}

