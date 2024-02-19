/**
 * Includes all system helper functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */


//===============================================================
// Inlcudes
//===============================================================
#include "SystemHelper.h"


//===============================================================
// Returns the complete system info as string
//===============================================================
String GetSystemInfoString()
{
  String ss;
  
  uint32_t chipId = 0;
	for(int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}
  
  // Chip-Information
  ss += "** Chip-Information: **";
  ss += "\nChip-ID:       " + String(chipId, HEX);
  ss += "\nModel:         " + String(ESP.getChipModel());
  ss += "\nRevision:      " + String(ESP.getChipRevision());
  ss += "\n";

  // CPU-Information
  ss += "\n** CPU-Information: **";
  ss += "\nCPU-Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz";
  ss += "\nCPU Count:     " + String(ESP.getChipCores());
  ss += "\n";

  // Storageinformation
  ss += "\n** Storage-Information: **";
  ss += "\nFlash-Size:    " + String((double)ESP.getFlashChipSize() / (1024.0 * 1024.0)) + " MB";
  ss += "\nSRAM-Size:     " + String((double)ESP.getFreeHeap() / (1024.0 * 1024.0)) + " MB";
  ss += "\nPRAM-Size:     " + String((double)ESP.getPsramSize() / (1024.0 * 1024.0)) + " MB";
  ss += "\n";

  // WLAN-Information
  ss += "\n** WLAN-Information: **";
  ss += "\nSSID:          " + String(WiFi.SSID());
  ss += "\nBSSID:         " + String(WiFi.BSSIDstr());
  ss += "\nChannel:       " + String(WiFi.channel());
  ss += "\nTX Power:      " + WifiPowerToString(WiFi.getTxPower());
  ss += "\n";

  return ss;
}

//===============================================================
// Returns a string for a wifi power
//===============================================================
String WifiPowerToString(wifi_power_t power)
{
  switch (power)
  {
    case WIFI_POWER_19_5dBm:
      return "19.5 dBm";
    case WIFI_POWER_19dBm:
      return "19 dBm";
    case WIFI_POWER_18_5dBm:
      return "18.5 dBm";
    case WIFI_POWER_17dBm:
      return "17 dBm";
    case WIFI_POWER_15dBm:
      return "15 dBm";
    case WIFI_POWER_13dBm:
      return "13 dBm";
    case WIFI_POWER_11dBm:
      return "11 dBm";
    case WIFI_POWER_8_5dBm:
      return "8.5 dBm";
    case WIFI_POWER_7dBm:
      return "7 dBm";
    case WIFI_POWER_5dBm:
      return "5 dBm";
    case WIFI_POWER_2dBm:
      return "2 dBm";
    case WIFI_POWER_MINUS_1dBm:
      return "-1 dBm";
    default:
      return "Unknown";
  }
}
