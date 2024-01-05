#include "esp32-hal.h"
/**
 * Includes encoder and button functions
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
 */
 
#include "EncoderButtonDriver.h"

//===============================================================
// Constructor
//===============================================================
EncoderButtonDriver::EncoderButtonDriver(uint8_t pinEncoderOutA, uint8_t pinEncoderOutB, uint8_t pinEncoderButton, uint8_t pinBuzzer) :
  _pinEncoderOutA(pinEncoderOutA),
  _pinEncoderOutB(pinEncoderOutB),
  _pinEncoderButton(pinEncoderButton),
  _pinBuzzer(pinBuzzer)
{
}

//===============================================================
// Initializes the encoder and button driver
//===============================================================
void EncoderButtonDriver::Begin()
{
  // Initialize GPIOs
  pinMode(_pinEncoderOutA, INPUT_PULLUP);
  pinMode(_pinEncoderOutB, INPUT_PULLUP);
  pinMode(_pinEncoderButton, INPUT_PULLUP);
  pinMode(_pinBuzzer, OUTPUT);

  // Initialize buzzer
  digitalWrite(_pinBuzzer, LOW);
  
  // Check if button is pressed while initializing => cleaning mode
  _cleaningMode = !digitalRead(_pinEncoderButton);
}

//===============================================================
// Return true, if cleaning mode is active. Otherwise false
//===============================================================
bool EncoderButtonDriver::IsCleaningMode()
{
  return _cleaningMode;
}

//===============================================================
// Return true, if a button press is pending. Otherwise false
//===============================================================
bool EncoderButtonDriver::IsButtonPress()
{
  // Save current state
  bool isButtonPress = _isButtonPress;
  
  // Reset button press flag
  _isButtonPress = false;
  
  if (isButtonPress)
  {
    // Short beep sound
    tone(_pinBuzzer, 500, 40);
  }
  
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

    // Long beep sound
    tone(_pinBuzzer, 800, 500);
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
    _isButtonPress = true;
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
