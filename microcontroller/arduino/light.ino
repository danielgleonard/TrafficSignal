#include "light.h"

Light::Light(uint8_t controlPin, bool powerState) {
  pin = controlPin;
  pinMode(pin, OUTPUT);
  setState(powerState);
}
Light::Light(byte controlPin) {
  pin = controlPin;
  pinMode(pin, OUTPUT);
  setState(false);
}

void Light::setState(bool powerState) {
  if(powerState) {
    digitalWrite(pin, HIGH);
    state = powerState;
  } else {
    digitalWrite(pin, LOW);
    state = powerState;
  }
}

bool Light::getState() {
  return state;
}
