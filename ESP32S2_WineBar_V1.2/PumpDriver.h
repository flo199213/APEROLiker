/**
 * Includes all pump functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#ifndef PUMPDRIVER_H
#define PUMPDRIVER_H

//===============================================================
// Includes
//===============================================================
#include <Arduino.h>
#include "Config.h"
#include "FlowMeterDriver.h"


//===============================================================
// Class for handling pump driver functions
//===============================================================
class PumpDriver
{
  public:
    // Constructor
    PumpDriver();

    // Initializes the pump driver
    void Begin(uint8_t pinPump1, uint8_t pinPump2, uint8_t pinPump3);
    
    // Return the timestamp of the last user action
    uint32_t GetLastUserAction();

    // Enables pump output
    void IRAM_ATTR Enable();
    
    // Disables pump output
    void IRAM_ATTR Disable();

    // Return true, if the pumps are enabled. Otherwise false
    bool IsEnabled();

    // Sets pumps on or off
    void SetPumps(bool value1, bool value2, bool value3);
    
    // Should be called every < 50 ms
    void IRAM_ATTR Update();

  private:
    // Preferences variable
    Preferences _preferences;

    // Pin definitions
    uint8_t _pinPump1;
    uint8_t _pinPump2;
    uint8_t _pinPump3;

    // Timing values
    bool _isPumpEnabled = false;
    uint32_t _lastPumpCycleStart_ms = 0;
    bool _enablePump1 = false;
    bool _enablePump2 = false;
    bool _enablePump3 = false;
    
    // Timestamp of last user action
    uint32_t _lastUserAction = 0;

    // Disables pump output (internal)
    void DisableInternal();
};


//===============================================================
// Global variables
//===============================================================
extern PumpDriver Pumps;

#endif
