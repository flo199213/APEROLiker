/**
 * Includes all flow meter functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#ifndef FLOWMETERDRIVER_H
#define FLOWMETERDRIVER_H

//===============================================================
// Includes
//===============================================================
#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"


//===============================================================
// Defines
//===============================================================
#define FLOWRATE              0.00000416667   // 250 ml/min (pump specification @ 20V) => 5e-6 l/ms

#define KEY_FLOW_LIQUID1      "FlowLiquid1"   // Key name: Maximum string length is 15 bytes, excluding a zero terminator.
#define KEY_FLOW_LIQUID2      "FlowLiquid2"   // Key name: Maximum string length is 15 bytes, excluding a zero terminator.
#define KEY_FLOW_LIQUID3      "FlowLiquid3"   // Key name: Maximum string length is 15 bytes, excluding a zero terminator.

//===============================================================
// Class for flow measuring
//===============================================================
class FlowMeterDriver
{
  public:
    // Constructor
    FlowMeterDriver();
    
    // Load settings from EEPROM
    void Load();

    // Save settings to EEPROM
    void Save();

    // Returns current flow meter values
    double GetValueLiquid1();
    double GetValueLiquid2();
    double GetValueLiquid3();

    // Adds flow time (@100% pump power) to flow meter
    void IRAM_ATTR AddFlowTime(uint32_t valueLiquid1_ms, uint32_t valueLiquid2_ms, uint32_t valueLiquid3_ms);
    
  private:
    // Flow meter variables
    Preferences _preferences;
    double _valueLiquid1_L;
    double _valueLiquid2_L;
    double _valueLiquid3_L;
};


//===============================================================
// Global variables
//===============================================================
extern FlowMeterDriver FlowMeter;


#endif
