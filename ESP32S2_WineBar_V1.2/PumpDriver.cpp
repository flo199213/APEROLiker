/**
 * Includes pump driver functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#include "PumpDriver.h"

//===============================================================
// Global variables
//===============================================================
PumpDriver Pumps;

//===============================================================
// Constructor
//===============================================================
PumpDriver::PumpDriver()
{
}

//===============================================================
// Initializes the pump driver
//===============================================================
void PumpDriver::Begin(uint8_t pinPump1, uint8_t pinPump2, uint8_t pinPump3)
{
  // Set pins
  _pinPump1 = pinPump1;
  _pinPump2 = pinPump2;
  _pinPump3 = pinPump3;

  // Disable pump output
  DisableInternal();

  // Reset timestamp of last user action
  _lastUserAction = millis();
}

//===============================================================
// Return the timestamp of the last user action
//===============================================================
uint32_t PumpDriver::GetLastUserAction()
{
  return _lastUserAction;
}

//===============================================================
// Enables pump output
//===============================================================
void PumpDriver::Enable()
{
  if (_isPumpEnabled)
  {
    return;
  }

  // Set pins to output direction (enable)
  pinMode(_pinPump1, OUTPUT);
  pinMode(_pinPump2, OUTPUT);
  pinMode(_pinPump3, OUTPUT);
 
  // Set enabled flag to true
  // -> Update function is unlocked
  _isPumpEnabled = true;

  // Set timestamp of last user action
  _lastUserAction = millis();
}

//===============================================================
// Disables pump output
//===============================================================
void PumpDriver::Disable()
{
  if (!_isPumpEnabled)
  {
    return;
  }
  
  // Set enabled flag to false 
  // -> Update function is locked
  _isPumpEnabled = false;

  DisableInternal();

  // Set timestamp of last user action
  _lastUserAction = millis();
}

//===============================================================
// Disables pump output (internal)
//===============================================================
void PumpDriver::DisableInternal()
{
  // Set pins to input direction (disable)
  pinMode(_pinPump1, INPUT);
  pinMode(_pinPump2, INPUT);
  pinMode(_pinPump3, INPUT);
  
  // Disable pumps, just to be sure
  digitalWrite(_pinPump1, LOW);
  digitalWrite(_pinPump2, LOW);
  digitalWrite(_pinPump3, LOW);

  // Save timestamp
  uint32_t onTimestamp_ms = _lastPumpCycleStart_ms;
  uint32_t offTimestamp_ms = millis();

  // Add already passed flow time to flow meter if pump is not already off
  uint32_t passedFlowTime = offTimestamp_ms - onTimestamp_ms;
  uint32_t flowTime1_ms = _enablePump1 == true ? passedFlowTime : 0;
  uint32_t flowTime2_ms = _enablePump2 == true ? passedFlowTime : 0;
  uint32_t flowTime3_ms = _enablePump3 == true ? passedFlowTime : 0;
  FlowMeter.AddFlowTime(flowTime1_ms, flowTime2_ms, flowTime3_ms);
}

//===============================================================
// Return true, if the pumps are enabled. Otherwise false
//===============================================================
bool PumpDriver::IsEnabled()
{
  return _isPumpEnabled;
}

//===============================================================
// Sets pumps on or off
//===============================================================
void PumpDriver::SetPumps(bool value1, bool value2, bool value3)
{
  // Set values
  _enablePump1 = value1;
  _enablePump2 = value2;
  _enablePump3 = value3;
}

//===============================================================
// Should be called every < 50 ms
//===============================================================
void PumpDriver::Update()
{
  if (!_isPumpEnabled)
  {
    return;
  }

  // Write digital pins
  digitalWrite(_pinPump1, _enablePump1 ? HIGH : LOW);
  digitalWrite(_pinPump2, _enablePump2 ? HIGH : LOW);
  digitalWrite(_pinPump3, _enablePump3 ? HIGH : LOW);
  
  // Save absolute time for pwm calculations
  uint32_t absoluteTime_ms = millis();

  // Calculate relative time
  uint32_t relativeTime_ms = absoluteTime_ms - _lastPumpCycleStart_ms;

  // Add flow times when powering off (falling edge)
  uint32_t flowTime1_ms = _enablePump1 == true ? relativeTime_ms : 0;
  uint32_t flowTime2_ms = _enablePump2 == true ? relativeTime_ms : 0;
  uint32_t flowTime3_ms = _enablePump3 == true ? relativeTime_ms : 0;
  FlowMeter.AddFlowTime(flowTime1_ms, flowTime2_ms, flowTime3_ms);

  _lastPumpCycleStart_ms = absoluteTime_ms;
}
