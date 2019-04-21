#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "settings.h"
//#include "CommandParser.h"

#define DEBUG true
#define MESSAGE_SIZE 32

// Start a TCP Server on port 5045
WiFiServer server(port);
WiFiClient client;

// NTP Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.illinois.edu", -18000L, 172800000UL);

// vars for message storage
char messageWiFiIn[MESSAGE_SIZE];
char messageWiFiOut[MESSAGE_SIZE];
char messageSerialIn[MESSAGE_SIZE];
char messageSerialOut[MESSAGE_SIZE];

void setup()
{
  initHardware();
  setupWiFi();
  timeClient.begin();

  if (!MDNS.begin(domain)) {
#if DEBUG
    Serial.println("Error setting up MDNS responder!");
#endif
    while (1) {
      delay(1000);
    }
  }
#if DEBUG
  Serial.println("mDNS responder started");
  Serial.println(domain);
#endif

  server.begin();

  // Add service to MDNS-SD
  MDNS.addService("raw", "tcp", port);
}

void loop()
{
  timeClient.update();
  MDNS.update();

  if (!client.connected()) {
    client = server.available();
    digitalWrite(0, LOW);
  } else {
    digitalWrite(0, HIGH);
    
    if(readMessages()) {
      processMessages();
      sendMessages();
    }

    /*
        while (client.available() > 0) {
          // Read incoming message
          char inChar = client.read();
          // Echo input on Serial monitor
          Serial.write(inChar);
        }
        while (Serial.available() > 0) {
          char outChar = Serial.read();
          client.write(outChar);
        }
    */
  }
}

bool readMessages()
{
  bool readanything = false;

  // Read message from TCP/IP
  if (client.available() > 0) {
    memset(messageWiFiIn,0,sizeof(messageWiFiIn));
    memset(messageSerialIn,0,sizeof(messageSerialIn));
    memset(messageWiFiOut,0,sizeof(messageWiFiOut));
    memset(messageSerialOut,0,sizeof(messageSerialOut));
    
#if DEBUG
    Serial.write("client.availabile: ");
    Serial.print(client.available());
    Serial.write("\r\n");
#endif
    
    char inChar = client.read();
    int i = 0;

    // Fill messageWiFiIn with client.read()
    while (inChar != '\r' && inChar != '\n' && strlen(messageWiFiIn) < sizeof(messageWiFiIn) - 1) {
      messageWiFiIn[i] = inChar;
      i++;
      inChar = client.read();
    }
    
    // dump rest of buffer if there's more
    if (strlen(messageWiFiIn) == sizeof(messageWiFiIn) - 1 || inChar == '\r') {
      while (inChar != '\n') {
        inChar = client.read();
      }
    }
    
#if DEBUG
    Serial.write("messageWiFiIn:\"");
    Serial.write(messageWiFiIn);
    Serial.write("\" (start: ");
    Serial.print(messageWiFiIn[0]);
    Serial.write(")\r\n");
#endif

    readanything = true;
  }

  // Read message from Serial
  if (Serial.available() > 0) {
    memset(messageWiFiIn,0,sizeof(messageWiFiIn));
    memset(messageSerialIn,0,sizeof(messageSerialIn));
    memset(messageWiFiOut,0,sizeof(messageWiFiOut));
    memset(messageSerialOut,0,sizeof(messageSerialOut));

#if DEBUG
    Serial.write("serial.availabile: ");
    Serial.print(Serial.available());
    Serial.write("\r\n");
#endif

    char inChar = Serial.read();
    int i = 0;

    // Fill messageWiFiIn with client.read()
    while (inChar != '\r' && inChar != '\n' && strlen(messageSerialIn) < sizeof(messageSerialIn) - 1) {
      messageSerialIn[i] = inChar;
      i++;
      inChar = Serial.read();
    }

    // dump rest of buffer if there's more
    if (strlen(messageSerialIn) == sizeof(messageSerialIn) - 1 || inChar == '\r') {
      while (inChar != '\n') {
        inChar = Serial.read();
      }
    }    
#if DEBUG
    Serial.write("messageSerialIn:\"");
    Serial.write(messageSerialIn);
    Serial.write("\" (start: ");
    Serial.print(messageSerialIn[0]);
    Serial.write(")\r\n");
#endif

    readanything = true;
  }

#if DEBUG
    if (readanything) {
      Serial.write("read things lol\r\n");
    }
#endif

  return readanything;
}

void processMessages() {
  // check for commands to esp
  if (messageWiFiIn[0] != '\0' && processCommands(messageWiFiIn) == 2) {
    strncpy(messageSerialOut, messageWiFiIn+5, MESSAGE_SIZE);
  }
  if (messageSerialIn[0] != '\0' && processCommands(messageSerialIn) == 2) {
    strncpy(messageWiFiOut, messageSerialIn+5, MESSAGE_SIZE);
  }
}

char processCommands(char* msg) {
if (msg[0] == 'e' && msg[1] == 's' && msg[2] == 'p' && msg[3] == ' ') {
    return 1;
  } else if (msg[0] == 's' && msg[1] == 'e' && msg[2] == 'n' && msg[3] == 'd' && msg[4] == ' ') {
#if DEBUG
    Serial.write("MATCHES \'send\'.\r\n");
#endif
    return 2;
  }
  return 0;
}

void sendMessages() {
  if(messageSerialOut[0] != '\0') {
    Serial.write("client:\"");
    Serial.write(messageSerialOut);
    Serial.write("\"\r\n");
  }

  if(messageWiFiOut[0] != '\0') {
    client.write("serial:\"");
    client.write(messageWiFiOut);
    client.write("\"\r\n");
  }
}

void setupWiFi()
{
  WiFi.hostname(domain);
  WiFi.begin(ssid, password);
  //Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if DEBUG
    Serial.print(".");
#endif
  }
#if DEBUG
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
#endif
}

void initHardware()
{
  // LED on-board
  pinMode(0, OUTPUT);
  Serial.begin(57600);
}
