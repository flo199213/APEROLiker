/**
 * Includes encoder and button functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#include "EncoderButtonDriver.h"

//===============================================================
// Global variables
//===============================================================
 EncoderButtonDriver EncoderButton;

//===============================================================
// Constructor
//===============================================================
EncoderButtonDriver::EncoderButtonDriver()
{
}

//===============================================================
// Initializes the encoder and button driver
//===============================================================
void EncoderButtonDriver::Begin(uint8_t pinEncoderOutA, uint8_t pinEncoderOutB, uint8_t pinEncoderButton)
{
  // Set pins
  _pinEncoderOutA = pinEncoderOutA;
  _pinEncoderOutB = pinEncoderOutB;
  _pinEncoderButton = pinEncoderButton;

  // Initialize GPIOs
  pinMode(_pinEncoderOutA, INPUT_PULLUP);
  pinMode(_pinEncoderOutB, INPUT_PULLUP);
  pinMode(_pinEncoderButton, INPUT_PULLUP);
  
  // Reset timestamp of last user action
  _lastUserAction = millis();
}

//===============================================================
// Return the timestamp of the last user action
//===============================================================
uint32_t EncoderButtonDriver::GetLastUserAction()
{
  return _lastUserAction;
}

//===============================================================
// Return true, if a button press is pending. Otherwise false
//===============================================================
bool EncoderButtonDriver::IsButtonPress()
{
  // Save current state
  bool isButtonPress = _isButtonPress;
  
  if (_isButtonPress)
  {
    // Debounce
    delay(100);
    
    // Set timestamp of last user action
    _lastUserAction = millis();
  }

  // Reset button press flag
  _isButtonPress = false;
  
  // Return result
  return isButtonPress;
}

//===============================================================
// Return true, if a long button press is pending. Otherwise false
//===============================================================
bool EncoderButtonDriver::IsLongButtonPress()
{
  // Save current state
  uint32_t lastButtonPress_ms = _lastButtonPress_ms;
 
  // Check if long press condition is true (button currently pressed & time longer than threshold)
  bool isLongButtonPress = !digitalRead(_pinEncoderButton) && (millis() - lastButtonPress_ms) >= MINIMUMLONGTIMEPRESS_MS;
    
  if (isLongButtonPress)
  {
    // Reset button press timestamp
    _lastButtonPress_ms = 0;

    // Suppress upcoming next short button press (Long button press appears while pressing
    // the button. Next button press release is therefore no short button press)
    _suppressShortButtonPress = true;
    
    // Set timestamp of last user action
    _lastUserAction = millis();
  }
  
  // Return result
  return isLongButtonPress;
}

//===============================================================
// Returns the counted encoder pulses since the last query and 
// resets the counter
//===============================================================
int16_t EncoderButtonDriver::GetEncoderIncrements()
{
  // Save current increments value
  int16_t currentEncoderIncrements = _encoderIncrements;
  
  // Reset the counter
  _encoderIncrements = 0;

  if (currentEncoderIncrements != 0)
  {
    // Set timestamp of last user action
    _lastUserAction = millis();
  }

  // Return increments
  return currentEncoderIncrements;
}

//===============================================================
// Interrupt on button changing state
//===============================================================
void EncoderButtonDriver::ButtonEvent()
{
  // Check for falling edge (Pressed)
  if (!digitalRead(_pinEncoderButton))
  {
    // Save press button timestamp
    _lastButtonPress_ms = millis();
  }
  // Check for rising edge (Released)
  else
  {
    // Show short button press
    // Is linked with the suppress flag (flag is true, if it was a long button press and we have to discard this)
    _isButtonPress = !_suppressShortButtonPress;
    _suppressShortButtonPress = false;
  }
}

//===============================================================
// Should be called if encoder edge A changed
//===============================================================
void EncoderButtonDriver::DoEncoderA()
{
  if (digitalRead(_pinEncoderOutA) != _A_set)
  {
    _A_set = !_A_set;
    
    // Adjust counter -1 if A leads B
    if (_A_set && !_B_set) 
    {
      _encoderIncrements = _encoderIncrements - 1;
    }
  }
}

//===============================================================
// Should be called if encoder edge B changed
//===============================================================
void EncoderButtonDriver::DoEncoderB()
{
  if (digitalRead(_pinEncoderOutB) != _B_set)
  {
    _B_set = !_B_set;
    
    //  Adjust counter +1 if B leads A
    if (_B_set && !_A_set) 
    {
      _encoderIncrements = _encoderIncrements + 1;
    }
  }
}
