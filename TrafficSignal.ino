boolean lights[(byte)3];

void setup() {
    allState(false);
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}

void loop() {
    
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

}