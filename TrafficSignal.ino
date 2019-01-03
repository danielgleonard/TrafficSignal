#include "light.h"

Light lightGreen(2);
Light lightYellow(3);
Light lightRed(4);

void setup() {
  Serial.begin(9600);
  // Send test pattern
  lightDispTest();
}

void loop() {
  lightDispPatternRandom(250);
}
