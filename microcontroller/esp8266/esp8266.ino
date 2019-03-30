#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include "settings.h"
//#include "CommandParser.h"

#define DEBUG true

// Start a TCP Server on port 5045
WiFiServer server(port);
WiFiClient client;

void setup() 
{
  initHardware();
  setupWiFi();
  
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
  MDNS.update();
  if (!client.connected()) {
    client = server.available();
    digitalWrite(0,LOW);
  } else {
    digitalWrite(0,HIGH);
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
  Serial.begin(115200);
}
