#include <string.h>
#include "light.h"

#define MAX_INPUT 32

boolean lights[(byte)3];
const byte green = 0;
const byte yellow = 1;
const byte red = 2;

char message[MAX_INPUT];
// char messageIn[MAX_INPUT];
char messageOut[MAX_INPUT];
char messageToken[MAX_INPUT];
char messageArgv[MAX_INPUT][MAX_INPUT];

void setup() {
  Serial.begin(57600);
}

void loop() {
  while (Serial.available () > 0) {
    processIncomingByte(Serial.read ());
  }
}

void interpret(char* msg) {
  memset(message,0,sizeof(message));
  memset(messageOut,0,sizeof(messageOut));
  memset(messageToken,0,sizeof(messageToken));
  
  if(msg[0] == 'e' && msg[1] == 's' && msg[2] == 'p' && msg[3] == ':' && msg[4] == '"') {
    interpret_esp(msg);
  } else if (msg[0] == 'c' && msg[1] == 'l' && msg[2] == 'i' && msg[3] == 'e' && msg[4] == 'n' && msg[5] == 't' && msg[6] == ':' && msg[7] == '"') {
    interpret_client(msg);
  }
}

void interpret_esp(char* msg) {
  if (subString(msg, 5, strlen(msg) - 1 - 5, message)) {
    // MESSAGE FROM ESP HERE
  }
}

void interpret_client(char* msg) {
  if (subString(msg, 8, strlen(msg) - 1 - 8, message)) {
    int i = 0;
    // MESSAGE FROM CLIENT HERE
    const char s[2] = " ";

    Serial.write(message);
    Serial.write("\r\n");

    strncpy(messageToken,message,MAX_INPUT);
    
    strtok(messageToken, s);

    Serial.write("messageToken == \"");
    Serial.write(messageToken);
    Serial.write("\"\r\n");

    Serial.write(message);
    subString(message, strlen(messageToken), strlen(message) - 1 - strlen(messageToken), message);
    Serial.write(message);

/*
    while(messageToken != NULL) {
      memset(messageArgv[i],0,sizeof(messageArgv[i]));
      strncpy(messageArgv[i],messageToken,MAX_INPUT);
      subString(message, strlen(messageToken), strlen(message) - 1 - strlen(messageToken), message);

      memset(messageToken,0,sizeof(messageToken));
      strncpy(messageToken, message, MAX_INPUT);
      strtok(messageToken, s);

      Serial.write("messageArgv[");
      Serial.print(i);
      Serial.write("] == \"");
      Serial.write(messageArgv[i]);
      Serial.write("\"\r\n");

      i++;
    }*/
    
    /*

    const char s[2] = " ";

    strncpy(messageToken, strtok(message, s), MAX_INPUT);
    
    while(messageToken != NULL ) {
      Serial.write( " %s\n", messageToken );
      
      memset(messageToken,0,sizeof(messageToken));
    
      strncpy(messageToken, strtok(message, s), MAX_INPUT);
    }
    */
  }
}


char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  dest[len+1] = '\0';
  return dest;
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
