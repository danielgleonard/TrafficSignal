/*
 * This header file allows you to change WiFi settings independently
 * of the main codebase. If you intend to make changes to the main
 * codebase while using this file as part of a working project or
 * in debugging, you can make Git ignore this file in committing with
 * 
 *     git update-index --skip-worktree network_info.h
 */

#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

// your network connection protocol (0 = open, 1 = WEP, 2 = WPA2 Personal)
#define NETWORK_TYPE 0

uint8_t net_ssid[] = "";        // your network SSID (name)

#if NETWORK_TYPE == 1
  // WEP
  uint8_t net_key[] = "";       // your network key
  int net_keyIndex = 0;         // your network key Index number
#elif NETWORK_TYPE == 2
  // WPA2 Personal
  uint8_t net_password[] = "";  // your network password
#endif

#endif
