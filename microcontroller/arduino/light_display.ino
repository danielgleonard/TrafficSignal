/*
void lightDispTest() {
  lightDispAllState(true);
  delay(100);
  lightDispAllState(false);
  delay(100); 
}

void lightDispPatternRandom(int delayTime) {
  byte randomNumber = random(3);
  lightDispToggleState(randomNumber);
  delay(delayTime);
}

void lightDispPatternMarquee(boolean up, int delayTime) {
  if (up) {
    lightDispToggleState(2);
    delay(delayTime);
    lightDispToggleState(3);
    delay(delayTime);
    lightDispToggleState(4);
    delay(delayTime);
  } else {
    lightDispToggleState(4);
    delay(delayTime);
    lightDispToggleState(3);
    delay(delayTime);
    lightDispToggleState(2);
    delay(delayTime);
  }
}

void lightDispToggleState(Light light) {
    lightDispSetState(light, !lightDispGetState(light));
}

void lightDispAllState(boolean state) {
    for (int i = 0; i < 3; i++) {
        lightDispSetState(i, false);
    }
}

*/
