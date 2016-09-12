#include "Switch.h"
#include "Logging.h"
#define DEBOUNCE_MILLIS 50

Switch::Switch(int pin) : pin_(pin), released_(false), changed_(false), state_(1) {
  pinMode(pin, INPUT_PULLUP);
  Log_Debug(String("Initialized Switch for ")+pin);
}

Switch::~Switch() {
  // Nothing to do
}

void Switch::refresh() {

  int curState = digitalRead(pin_);
  if (curState == HIGH) {
    lastReadHigh_ = millis();
  } else if (curState == LOW) {
    lastReadLow_ = millis();
  } else {
    return; // Not being able to read?
  }

  long timingDifference = (lastReadHigh_ - lastReadLow_);
  if (!(timingDifference >= DEBOUNCE_MILLIS || timingDifference <= -DEBOUNCE_MILLIS)) {
    // We have not debounced anything, so let's not do anything
    return;
  }
  // Now check the debounced state
  if (state_ != curState) {
    // There was a change
    changed_ = true;
    released_ = (state_ == LOW) && (curState == HIGH);
    Log_Debug(String("Button ")+pin_+" was "+(released_ ? "released" : "pressed"));
  } else {
    released_ = false;
    changed_ = false;
  }
  state_ = curState;
}

bool Switch::wasReleased() {
  return released_;
}

bool Switch::wasChanged() {
  return changed_;
}

bool Switch::getState() {
  return (this->state_ == LOW);
}

