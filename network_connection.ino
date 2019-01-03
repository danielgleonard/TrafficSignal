#include <SPI.h>
#include <WiFi.h>
#include "network_info.h"

int status = WL_IDLE_STATUS;

WiFiServer server(80);

bool connectWifi() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    return false;
  } else {
    String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {
      Serial.println("Please upgrade the firmware");
    }

    /*
     * Connect to network
     * Performs different procedure based on NETWORK_TYPE
     * using #if directives to save space on microcontroller
     */
#if NETWORK_TYPE == 0

#elif NETWORK_TYPE == 1
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WEP network, SSID: ");
      Serial.println(ssid);
      status = WiFi.begin(ssid, keyIndex, key);
  
      // wait 10 seconds for connection:
      delay(10000);
    }
#elif NETWORK_TYPE == 2
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
  
      // wait 10 seconds for connection:
      delay(10000);
    }
    server.begin();
    // you're connected now, so print out the status:
    printWifiStatus();      
#else
    return false;
#endif
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
