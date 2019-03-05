#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#define DEBUG true

// Hardcode WiFi parameters as this isn't going to be moving around.
const char* domain = "esp8266-light";
const char* ssid = "abwifi";
const char* password = "ABASMASB1234";
const int   port  = 5045;

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
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  Serial.println(domain);

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
    }
}

void readCommand(String command)
{
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
}

void setupWiFi()
{
  WiFi.begin(ssid,password);
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

void initHardware()
{
  Serial.begin(115200);
}
