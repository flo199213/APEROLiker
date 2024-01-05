/**
 * Includes all pump functions
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
 */
 
#ifndef PUMPDRIVER_H
#define PUMPDRIVER_H

//===============================================================
// Includes
//===============================================================
#include "Config.h"
#include <Arduino.h>


//===============================================================
// Defines
//===============================================================
#define CYCLETIME_MS    1000

//===============================================================
// Class for handling pump driver functions
//===============================================================
class PumpDriver
{
  public:
    // Constructor
    PumpDriver(uint8_t pinPump1, uint8_t pinPump2, uint8_t pinPump3);

    // Initializes the pump driver
    void Begin(bool isCleaningMode);

    // Enables pump output
    void IRAM_ATTR Enable();
    
    // Disables pump output
    void IRAM_ATTR Disable();

    // Return true, if the pumps are enabled. Otherwise false
    bool IsEnabled();

    // Sets pumps from percentage (0-100%)
    void UpdatePercentages(double pump1_percentage, double pump2_percentage, double pump3_percentage);
    
    // Should be called every < 50 ms
    void IRAM_ATTR UpdateOutputs();

  private:
    uint8_t _pinPump1;
    uint8_t _pinPump2;
    uint8_t _pinPump3;
    
    bool _isPumpEnabled = false;
    bool _isCleaningMode = false;
    
    uint32_t _pwmPump1_ms;
    uint32_t _pwmPump2_ms;
    uint32_t _pwmPump3_ms;
    uint32_t lastPumpCycle_ms = 0;
    
    // Set pump channel values
    void SetPWM(int pwm1, int pwm2, int pwm3);
};

#endif
