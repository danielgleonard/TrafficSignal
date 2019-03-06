#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include "settings.h"

#define DEBUG true

// Start a TCP Server on port 5045
WiFiServer server(port);
WiFiClient client;
String msg_in = "";

void setup() 
{
  initHardware();
  setupWiFi();
  
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
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
      // try to connect to a new client
      client = server.available();
  } else {
      // read string from the connected client
      if (client.available() > 0) {
        while (client.available() > 0) {
#if DEBUG
          Serial.print("Incoming characters: ");
          Serial.println(client.available());
#endif
          char s = (char)client.read();
          if (s == '\n') {
#if DEBUG
            Serial.println("Received newline");
#endif
            readCommand(msg_in);
            //char buf[msg_in.length()];
            //msg_in.toCharArray(buf, msg_in.length());
#if DEBUG
            Serial.print("Received line: ");
            Serial.println(msg_in);
#endif
            msg_in = "";
          } else if (s == '\r') {
#if DEBUG
            Serial.println("Received carriage return");
#endif
          } else {
#if DEBUG
            Serial.print("Received character: ");
            Serial.println(s);
#endif
            msg_in += s;
#if DEBUG
            Serial.print("Current understanding of line: ");
            Serial.println(msg_in);
#endif
        }
      }
      client.print(domain);
      client.print('>');
      client.print('\027');
    }
  }
}

void readCommand(String command)
{
  String command_arg = "";
  
  command.trim();

  if (command.indexOf(' ') == -1) {
    command_arg = command;
  } else {
    command_arg = command.substring(0, command.indexOf(' '));
  }
  command_arg.toLowerCase();
  
  if (command_arg.equals("help")) {
    
#if DEBUG
    Serial.println("Received command \"help\". Sending help to client.");
#endif
    client.println("TrafficLight v2 by Dan Leonard.\r\nAvailable commands:\r\n\tesp   [args]   -- send command [args] to WiFi board\r\n\tlight [args]   -- send command [args] to traffic light Arduino");

  } else if (command_arg.equals("esp")) {
#if DEBUG
    Serial.println("Received command \"esp\". Interpreting rest of command.");
#endif

  } else if (command_arg.equals("light")) {
#if DEBUG
    Serial.println("Received command \"light\". Sending onwards.");
#endif
    
    // SEND TO SERIAL
    String command_light = command.substring(command.indexOf(' ') + 1);
    Serial.println("client:" + '\002' + command_light + '\003');
    
  } else {
#if DEBUG
    Serial.print("Unknown command. Received: ");
    Serial.println(command);
    Serial.print("Understood as: ");
    Serial.println(command_arg);
#endif
    client.print("Unknown command: ");
    client.println(command_arg);
    client.println("Send command \"help\" for help");
  }
    
  return;

/*
  if (command.substring(0,3).equalsIgnoreCase("set")) {
#if DEBUG
    Serial.println("Received command \"set\"");
#endif
    client.println("Received command \"set\"");
    
    // Arguments for SET
    String com_args = command.substring(4);
#if DEBUG
    Serial.print("Received SET args: ");
    Serial.println(com_args);
#endif
    bool red;
    bool yellow;
    bool green;

    if(com_args.length() == 3){
      if((com_args[0] == '0' || com_args[0] == '1') && (com_args[1] == '0' || com_args[1] == '1') && (com_args[2] == '0' || com_args[2] == '1'))
      {
        switch (com_args[0]) {
        case '0':
          red = false;
          break;
        case '1':
          red = true;
          break;
        default:
#if DEBUG
          Serial.print("Unexpected error. SET argument 1 (red) was: ");
          Serial.println(com_args[0]);
#endif
          break;
        }
        switch (com_args[1]) {
        case '0':
          yellow = false;
          break;
        case '1':
          yellow = true;
          break;
        default:
#if DEBUG
          Serial.print("Unexpected error. SET argument 2 (yellow) was: ");
          Serial.println(com_args[1]);
#endif
          break;
        }
        switch (com_args[2]) {
        case '0':
          green = false;
          break;
        case '1':
          green = true;
          break;
        default:
#if DEBUG
          Serial.print("Unexpected error. SET argument 3 (green) was: ");
          Serial.println(com_args[2]);
#endif
          break;
        }
      } else {
#if DEBUG
          Serial.print("Malformed argument. SET takes three binary digits. Received: ");
          Serial.println(com_args);
#endif
          client.print("Malformed argument. SET takes three binary digits. Received: ");
          client.println(com_args);
        return;
      }
    } else {
#if DEBUG
          Serial.print("Malformed argument. SET takes three binary digits. Received: ");
          Serial.println(com_args);
#endif
          client.print("Malformed argument. SET takes three binary digits. Received: ");
          client.println(com_args);
      return;
    }
  }
  */
}

void setupWiFi()
{
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
  Serial.begin(115200);
}
