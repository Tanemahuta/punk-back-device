#include "LED.h"

#define NO_BLINK _blinkingTime = 0

LED::LED(int pin) {
  _pin = pin;
  pinMode(pin, OUTPUT);
  off();
}

LED::~LED() {
  // Nothing to do yet
}

void LED::setState(bool state) {
  _lastHandled = millis();
  _state = state;
  digitalWrite(_pin, _state);
}

bool LED::getState() {
  return _state;
}

void LED::on() {
  NO_BLINK;
  setState(HIGH);
}

void LED::off() {
  NO_BLINK;
  setState(LOW);
}

void LED::blink(int time) {
  setState(HIGH);
  _blinkingTime = time;
}

void LED::blinkBlocking(int time, int times) {
  for (int i = 0; i < times; i++) {
    on();
    delay(time);
    off();
    delay(time);
  }
}

void LED::negate() {
  setState(!_state);
}

void LED::handle() {
  if (_blinkingTime == 0) {
    return; // Nothing to do, mate
  }
  int cur = millis();
  int next = _lastHandled + _blinkingTime;
  if (next <= cur) {
    negate();
  }
}


