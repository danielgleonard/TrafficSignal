#include <SPI.h>
#include <WiFi.h>
#include "network_info.h"

int status = WL_IDLE_STATUS;

WiFiServer server(80);

bool connectWifi() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    return false;
  }
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
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open network, SSID: ");
    Serial.println(net_ssid);
    status = WiFi.begin(net_ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }
#elif NETWORK_TYPE == 1
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(net_ssid);
    status = WiFi.begin(net_ssid, net_keyIndex, net_key);

    // wait 10 seconds for connection:
    delay(10000);
  }
#elif NETWORK_TYPE == 2
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA2 Personal network, SSID: ");
    Serial.println(net_ssid);
    status = WiFi.begin(net_ssid, net_password);

    // wait 10 seconds for connection:
    delay(10000);
  }
#else
  Serial.println("Unknown network type, connection failed");
  return false;
#endif

  server.begin();
  return true;
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
