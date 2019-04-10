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
    digitalWrite(0,LOW);
  } else {
    digitalWrite(0,HIGH);
    readMessages();

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
  }
}

void readMessage()
{

	// Read message from TCP/IP
	while (client.available() > 0) {
		char inChar = client.read();
		int i = 0;

		while (inChar != '\r' && inChar != '\n' && strlen(messageWiFiIn) < sizeof(messageWiFiIn) - 1) {
			messageWiFiIn[i] = inChar;
			i++;
			inChar = client.read();
		}
		if (strlen(messageWiFiIn) == sizeof(messageWiFiIn) - 1) {
		}
	}
}

void setupWiFi()
{
  WiFi.hostname(domain);
  WiFi.begin(ssid,password);
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
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
