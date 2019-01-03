#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

// your network connection protocol (0 = open, 1 = WEP, 2 = WPA2 Personal)
#define NETWORK_TYPE 0

char net_ssid[] = "";       // your network SSID (name)

// WEP
char key[] = "";            // your network key
int keyIndex = 0;           // your network key Index number

// WPA2 Personal
char net_password[] = "";   // your network password

#endif
