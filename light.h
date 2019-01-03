#ifndef LIGHT_H
#define LIGHT_H

class Light {
  private:
    uint8_t pin;
    bool state;

  public:
    Light(uint8_t controlPin, bool powerState);
    Light(uint8_t controlPin);

    void setState(bool powerState);

    bool getState() { return state; }
}

#endif
