boolean lights[(byte)3];
const byte green = 0;
const byte yellow = 1;
const byte red = 2;

void setup() {
    pinMode(2, OUTPUT); //green
    pinMode(3, OUTPUT); //yellow
    pinMode(4, OUTPUT); //red
    
    allState(false);
}

void loop() {
  randomLight(250);
}

void randomLight(int delayTime) {
  byte randomNumber = random(3);
  toggleState(randomNumber);
  delay(delayTime);
}

void marquee(boolean up, int delayTime) {
  if (up) {
    toggleState(green);
    delay(delayTime);
    toggleState(yellow);
    delay(delayTime);
    toggleState(red);
    delay(delayTime);
  } else {
    toggleState(red);
    delay(delayTime);
    toggleState(yellow);
    delay(delayTime);
    toggleState(green);
    delay(delayTime);
  }
}

boolean getState(byte light) {
    return lights[light];
}

void toggleState(byte light) {
    setState(light, !getState(light));
}

void allState(boolean state) {
    for (int i = 0; i < 3; i++) {
        setState(i, false);
    }
}

void setState(byte light, boolean state) {
    if (state) {
        digitalWrite(light + 2, LOW);
    }
    else
    {
        digitalWrite(light + 2, HIGH);
    }

    lights[light] = state;
}
