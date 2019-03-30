#include "light.h"

Light lightGreen(2);
Light lightYellow(3);
Light lightRed(4);

void setup() {
  /*
  lightGreen  = new Light(2);
  lightYellow = new Light(3);
  lightRed    = new Light(4);
  */
  
  Serial.begin(115200);
  
  // Send test pattern
//  lightDispTest();
}

void loop() {
  if (Serial.available() > 0) {
    Serial.print(Serial.read());
  }
}
