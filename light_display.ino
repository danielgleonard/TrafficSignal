
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

boolean lightDispGetState(uint8_t light) {
    //return lights[light];
    return false;
}

void lightDispToggleState(uint8_t light) {
    lightDispSetState(light, !lightDispGetState(light));
}

void lightDispAllState(boolean state) {
    for (int i = 0; i < 3; i++) {
        lightDispSetState(i, false);
    }
}

void lightDispSetState(uint8_t light, boolean state) {
    if (state) {
        digitalWrite(light + 2, LOW);
    }
    else
    {
        digitalWrite(light + 2, HIGH);
    }
/*
    lights[light] = state;
    */
}
