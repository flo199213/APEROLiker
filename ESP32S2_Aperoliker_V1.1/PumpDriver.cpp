/**
 * Includes pump driver functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#include "PumpDriver.h"

//===============================================================
// Constructor
//===============================================================
PumpDriver::PumpDriver(uint8_t pinPump1, uint8_t pinPump2, uint8_t pinPump3) :
  _pinPump1(pinPump1),
  _pinPump2(pinPump2),
  _pinPump3(pinPump3)
{
}

//===============================================================
// Initializes the pump driver
//===============================================================
void PumpDriver::Begin()
{
  // Set pwm to zero
  SetPWM(0, 0, 0);
  
  // Reset pumps enable
  Disable();
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
  _isPumpEnabled = true;
 
  // Set pins to output direction (enable)
  pinMode(_pinPump1, OUTPUT);
  pinMode(_pinPump2, OUTPUT);
  pinMode(_pinPump3, OUTPUT);

  // Initialize to low level
  digitalWrite(_pinPump1, LOW);
  digitalWrite(_pinPump2, LOW);
  digitalWrite(_pinPump3, LOW);
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
  
  // Set pins to input direction (disable)
  pinMode(_pinPump1, INPUT);
  pinMode(_pinPump2, INPUT);
  pinMode(_pinPump3, INPUT);
  
  // Disable to low level
  digitalWrite(_pinPump1, LOW);
  digitalWrite(_pinPump2, LOW);
  digitalWrite(_pinPump3, LOW);
  
  _isPumpEnabled = false;
}

//===============================================================
// Return true, if the pumps are enabled. Otherwise false
//===============================================================
bool PumpDriver::IsEnabled()
{
  return _isPumpEnabled;
}

//===============================================================
// Sets pumps from percentage
//===============================================================
void PumpDriver::UpdatePercentages(double pump1_percentage, double pump2_percentage, double pump3_percentage, bool maximizeHighest)
{
  uint32_t pwm1_ms;
  uint32_t pwm2_ms;
  uint32_t pwm3_ms;

  if (maximizeHighest)
  {
    // Calculate pwm timings (pump with the highest value is set
    // to 100% pwm and the other two in relative to the max one)
    double maxValue = max(1.0, max(pump1_percentage, max(pump2_percentage, pump3_percentage))); // 1.0->avoid divison by zero if all values are zero
    pwm1_ms = (uint32_t)(pump1_percentage / maxValue * CYCLETIME_MS);
    pwm2_ms = (uint32_t)(pump2_percentage / maxValue * CYCLETIME_MS);
    pwm3_ms = (uint32_t)(pump3_percentage / maxValue * CYCLETIME_MS);
  }
  else
  {
    // Optional: Set only percentage values
    pwm1_ms = (uint32_t)(pump1_percentage / 100.0 * CYCLETIME_MS);
    pwm2_ms = (uint32_t)(pump2_percentage / 100.0 * CYCLETIME_MS);
    pwm3_ms = (uint32_t)(pump3_percentage / 100.0 * CYCLETIME_MS);
  }

  SetPWM(pwm1_ms, pwm2_ms, pwm3_ms);
}

//===============================================================
// Set pwm channel values
//===============================================================
void PumpDriver::SetPWM(uint32_t pwm1, uint32_t pwm2, uint32_t pwm3)
{  
  // Check min and max borders
  // Invert value, so _pwmPumpX_ms is the amount of time the pump is off
  _pwmPump1_ms = CYCLETIME_MS - min(max(pwm1, (uint32_t)0), CYCLETIME_MS);
  _pwmPump2_ms = CYCLETIME_MS - min(max(pwm2, (uint32_t)0), CYCLETIME_MS);
  _pwmPump3_ms = CYCLETIME_MS - min(max(pwm3, (uint32_t)0), CYCLETIME_MS);
}

//===============================================================
// Should be called every < 50 ms
//===============================================================
void PumpDriver::UpdateOutputs()
{
  if (!_isPumpEnabled)
  {
    return;
  }

  // New cycle starts, shut off all pumps
  if ((millis() - lastPumpCycle_ms) > CYCLETIME_MS)
  {
    lastPumpCycle_ms = millis();
    digitalWrite(_pinPump1, LOW);
    digitalWrite(_pinPump2, LOW);
    digitalWrite(_pinPump3, LOW);
  }

  // Check if pump 1 must be powered on
  if ((millis() - lastPumpCycle_ms) > _pwmPump1_ms)
  {
    digitalWrite(_pinPump1, HIGH);
  }

  // Check if pump 2 must be powered on
  if ((millis() - lastPumpCycle_ms) > _pwmPump2_ms)
  {
    digitalWrite(_pinPump2, HIGH);
  }
  
  // Check if pump 3 must be powered on
  if ((millis() - lastPumpCycle_ms) > _pwmPump3_ms)
  {
    digitalWrite(_pinPump3, HIGH);
  }
}
