#include "light.h"

#define MAX_INPUT 32

boolean lights[(byte)3];
const byte green = 0;
const byte yellow = 1;
const byte red = 2;

void setup() {  
  Serial.begin(57600);
}

void loop() {
  while (Serial.available () > 0) {
    processIncomingByte(Serial.read ());
  }
}

interpret(char* msg) {
  
}

/**
 * Process the next byte coming in
 * 
 * Adapted from a forum post by 'Nick Gammon'
 * available at http://www.gammon.com.au/serial
 * 
 * @param inByte The byte coming in from Serial
 */
void processIncomingByte (const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;
  
  switch (inByte) {
    case '\n':   // end of text
      input_line[input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
      interpret(input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;
  
    case '\r':   // discard carriage return
      break;
  
    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line[input_pos++] = inByte;
      break;
  }  // end of switch
 
} // end of processIncomingByte  
