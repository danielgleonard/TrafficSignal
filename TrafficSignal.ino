boolean[] lights = new boolean[3];

void setup() {

}

void loop() {
  switch (light) {

  }
}

boolean getState(byte light) {
  switch (light) {
    case 0:
      return red;
    case 1:
      return yellow;
    case 2:
      return green;
    default:
      return false;
  }
}