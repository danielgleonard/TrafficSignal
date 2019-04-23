#include <string.h>
#include "light.h"

#define MAX_INPUT 32
#define DEBUG true

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
  
#if DEBUG
  Serial.write("Serial beginning.\r\n");
#endif
}

void loop() {
  while (Serial.available () > 0) {
    
#if DEBUG
    Serial.write("Bytes to read! ");
#endif
    
    processIncomingByte(Serial.read ());
  }
}

void interpret(char* msg) {
  memset(message,0,sizeof(message));
  memset(messageOut,0,sizeof(messageOut));
  memset(messageToken,0,sizeof(messageToken));
  
  if(msg[0] == 'e' && msg[1] == 's' && msg[2] == 'p' && msg[3] == ':' && msg[4] == '"') {
#if DEBUG
    Serial.write("Looks like the message is coming from an esp.\r\n");
#endif
    
    interpret_esp(msg);
  } else if (msg[0] == 'c' && msg[1] == 'l' && msg[2] == 'i' && msg[3] == 'e' && msg[4] == 'n' && msg[5] == 't' && msg[6] == ':' && msg[7] == '"') {
#if DEBUG
    Serial.write("Looks like the message is coming from a client.\r\n");
#endif
    
    interpret_client(msg);
  }
}

void interpret_esp(char* msg) {
  subString(msg, 5, strlen(msg) - 1 - 5, message);
  
  if (strlen(message) > 0) {
    // MESSAGE FROM ESP HERE
  }
}

void interpret_client(char* msg) {
  subString(msg, 8, strlen(msg) - 1 - 8, message);
  
  if (strlen(message) > 0) {
    int i = 0;
    // MESSAGE FROM CLIENT HERE
    const char s[2] = " ";
    
#if DEBUG
    Serial.write("const char s[2] = \"");
    Serial.write(s);
    Serial.write("\"\n");
    
    delay(1000);
#endif

    strncpy(messageToken,message,MAX_INPUT);

#if DEBUG
    Serial.write("strncpy(messageToken,message,MAX_INPUT);\r\n");
    
    delay(1000);
#endif
    
    strtok(messageToken, s);

#if DEBUG
    Serial.write("strtok(messageToken, s);\r\n");
    
    delay(1000);

    Serial.write("messageToken == \"");
    Serial.write(messageToken);
    Serial.write("\"\r\n");

    delay(1000);

    Serial.write("subString(\"");
    Serial.write(message);
    Serial.write("\", ");
    Serial.print(strlen(messageToken));
    Serial.write(", ");
    Serial.print(strlen(message) - 1 - strlen(messageToken));
    Serial.write(", message);\r\n");

    delay(1000);
#endif

    subString(message, strlen(messageToken), strlen(message) - 1 - strlen(messageToken), message);

#if DEBUG
    Serial.write("messsage == \"");
    Serial.write(message);
    Serial.write("\"\r\n");
    
    delay(1000);

    Serial.write("memset(\"");
    Serial.write(message+strlen(messageToken));
    Serial.write("\", 0, MAX_INPUT);\r\n");

    delay(1000);
#endif
    
    memset(message+strlen(messageToken), 0, MAX_INPUT);

    Serial.write(message);

#if DEBUG
    delay(1000);
#endif

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
  
#if DEBUG
  Serial.write("processIncomingByte(\'");
  switch (inByte) {
    case '\n':
      Serial.write("\\n");
      break;
    case '\r':
      Serial.write("\\r");
      break;
    default:
      Serial.write(inByte);
      break;
  }
  Serial.write("\')\r\n");
#endif
  
  switch (inByte) {
    case '\n':   // end of text
      input_line[input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
#if DEBUG
      Serial.write("Found a \'\\n\'!\r\n");
      Serial.write("Line to read: \"");
      Serial.write(input_line);
      Serial.write("\"\r\n");
#endif
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
