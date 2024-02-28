/**
 * Includes all state machine functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//===============================================================
// Includes
//===============================================================
#include <Arduino.h>
#include <WiFi.h>
#include "Config.h"
#include "EncoderButtonDriver.h"
#include "PumpDriver.h"
#include "DisplayDriver.h"
#include "FlowMeterDriver.h"
#include "WifiHandler.h"


//===============================================================
// Defines
//===============================================================
#define SCREENSAVER_TIMEOUT_MS      30000     // 30 seconds


//===============================================================
// Class for state machine handling
//===============================================================
class StateMachine
{
  public:
    // Constructor
    StateMachine();

    // Initializes the state machine
    void Begin(uint8_t pinBuzzer);

    // Returns the current mixer state of the state machine
    MixerState GetCurrentState();

    // General state machine execution function
    void Execute(MixerEvent event);

  private:
    // Pin definitions
    uint8_t _pinBuzzer;

    // State machine variables
    MixerState _currentState = eDashboard;
    MixerState _lastState = eDashboard;
    MixerState _currentMenuState = eDashboard;

    // Dashboard mode settings
    MixtureLiquid _dashboardLiquid = eLiquid1;

    // Cleaning mode settings
    MixtureLiquid _cleaningLiquid = eLiquidAll;

    // Timer variables for reset counter
    uint32_t _resetTimestamp = 0;
    const uint32_t ResetTime_ms = 2000;

    // Wifi client variables
    uint16_t _lastConnectedClients = 0;

    // Function menu state
    void FctMenu(MixerEvent event);

    // Function dashboard state
    void FctDashboard(MixerEvent event);

    // Function cleaning state
    void FctCleaning(MixerEvent event);

    // Function reset state
    void FctReset(MixerEvent event);

    // Function settings state
    void FctSettings(MixerEvent event);

    // Function screen saver state
    void FctScreenSaver(MixerEvent event);
    
    // Resets the mixture to default recipe
    void SetMixtureDefaults();

    // Updates all values in display, pumps driver and wifi
    void UpdateValues(uint32_t clientID = 0);
};


//===============================================================
// Global variables
//===============================================================
extern StateMachine Statemachine;


#endif
