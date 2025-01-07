/**
 * Includes all flow meter functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

//===============================================================
// Includes
//===============================================================
#include "FlowMeterDriver.h"

//===============================================================
// Global variables
//===============================================================
FlowMeterDriver FlowMeter;

//===============================================================
// Constructor
//===============================================================
FlowMeterDriver::FlowMeterDriver()
{
}

//===============================================================
// Load settings from flash
//===============================================================
void FlowMeterDriver::Load()
{
  if (_preferences.begin(SETTINGS_NAME, true))
  {
    _valueLiquid1_L = _preferences.getDouble(KEY_FLOW_LIQUID1, 0.0);
    _valueLiquid2_L = _preferences.getDouble(KEY_FLOW_LIQUID2, 0.0);
    _valueLiquid3_L = _preferences.getDouble(KEY_FLOW_LIQUID3, 0.0);
    _preferences.end();
  }
}

//===============================================================
// Save settings to flash
//===============================================================
void FlowMeterDriver::Save()
{
  if (_preferences.begin(SETTINGS_NAME, false))
  {
    _preferences.putDouble(KEY_FLOW_LIQUID1, _valueLiquid1_L);
    _preferences.putDouble(KEY_FLOW_LIQUID2, _valueLiquid2_L);
    _preferences.putDouble(KEY_FLOW_LIQUID3, _valueLiquid3_L); 
    _preferences.end();
  }
}

//===============================================================
// Returns true, if a save call is pending to be executed by the
// loop task
//===============================================================
void FlowMeterDriver::SaveAsync()
{
  if (_isSavePending)
  {
    _isSavePending = false;
    Save();
  }
}

//===============================================================
// Returns current flow meter value for liquid 1
//===============================================================
double FlowMeterDriver::GetValueLiquid1()
{
  return _valueLiquid1_L;
}

//===============================================================
// Returns current flow meter value for liquid 2
//===============================================================
double FlowMeterDriver::GetValueLiquid2()
{
  return _valueLiquid2_L;
}

//===============================================================
// Returns current flow meter value for liquid 3
//===============================================================
double FlowMeterDriver::GetValueLiquid3()
{
  return _valueLiquid3_L;
}

//===============================================================
// Adds flow time (@100% pump power) to flow meter
//===============================================================
void FlowMeterDriver::AddFlowTime(uint32_t valueLiquid1_ms, uint32_t valueLiquid2_ms, uint32_t valueLiquid3_ms)
{
  _valueLiquid1_L += (double)valueLiquid1_ms * FLOWRATE;
  _valueLiquid2_L += (double)valueLiquid2_ms * FLOWRATE;
  _valueLiquid3_L += (double)valueLiquid3_ms * FLOWRATE;
}

//===============================================================
// Requests a save values from interrupt service routine
//===============================================================
void FlowMeterDriver::RequestSaveAsync()
{
  _isSavePending = true;
}
