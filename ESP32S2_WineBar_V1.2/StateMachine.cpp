/**
 * Includes all state machine functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

#include "StateMachine.h"

//===============================================================
// Global variables
//===============================================================
StateMachine Statemachine;

//===============================================================
// Constructor
//===============================================================
StateMachine::StateMachine()
{
}

//===============================================================
// Initializes the state machine
//===============================================================
void StateMachine::Begin(uint8_t pinBuzzer)
{
  // Save pin definitions
  _pinBuzzer = pinBuzzer;

  // Update all values
  UpdateValues();
}

//===============================================================
// Returns the current mixer state of the state machine
//===============================================================
MixerState StateMachine::GetCurrentState()
{
  return _currentState;
}

//===============================================================
// General state machine execution function
//===============================================================
void StateMachine::Execute(MixerEvent event)
{
  switch (_currentState)
  {
    case eMenu:
      FctMenu(event);
      break;
    case eCleaning:
      FctCleaning(event);
      break;
    case eReset:
      FctReset(event);
      break;
    case eSettings:
      FctSettings(event);
      break;
    case eScreenSaver:
      FctScreenSaver(event);
      break;
    default:  // In case something went wrong, default case is dashboard
    case eDashboard:
      FctDashboard(event);
      break;
  }
}

//===============================================================
// Function menu state
//===============================================================
void StateMachine::FctMenu(MixerEvent event)
{
  switch (event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show menu page
        Serial.println("[MAIN] Enter Menu Mode");
        Display.ShowMenuPage();

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        EncoderButton.GetEncoderIncrements();
        EncoderButton.IsButtonPress();
      }
      break;
    case eMain:
      {
        // Read encoder increments (resets the counter value)
        int16_t currentEncoderIncrements = EncoderButton.GetEncoderIncrements();

        // Check for changed encoder value
        if (currentEncoderIncrements != 0)
        {
          // Increment or decrement current menu state
          switch (_currentMenuState)
          {
            case eDashboard:
              _currentMenuState = currentEncoderIncrements > 0 ? eDashboard : eCleaning;
              break;
            case eCleaning:
              _currentMenuState = currentEncoderIncrements > 0 ? eDashboard : eReset;
              break;
            case eReset:
              _currentMenuState = currentEncoderIncrements > 0 ? eCleaning : eSettings;
              break;
            case eSettings:
              _currentMenuState = currentEncoderIncrements > 0 ? eReset : eSettings;
              break;
            default:
              break;
          }
            
          // Update display and pump values
          UpdateValues();

          // Update menu
          Display.DrawMenu();
        }

#if defined(WIFI_MIXER)
        // Draw wifi icons
        Display.DrawWifiIcons();
#endif

        // Check for button press
        if (EncoderButton.IsButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 500, 40);

          // Exit menu and enter new selected mode
          Execute(eExit);
          _currentState = _currentMenuState;
          Execute(eEntry);
          return;
        }
        
        // Check for screen saver timeout
        if (millis() - EncoderButton.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS &&
          millis() - Pumps.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS)
        {
          // Exit menu mode and enter screen saver mode
          Execute(eExit);
          _lastState = eMenu;
          _currentState = eScreenSaver;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Function dashboard state
//===============================================================
void StateMachine::FctDashboard(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show dashboard page
        Serial.println("[MAIN] Enter Dashboard Mode");
        Display.ShowDashboardPage();

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        EncoderButton.GetEncoderIncrements();
        EncoderButton.IsLongButtonPress();
        EncoderButton.IsButtonPress();
      }
      break;
    case eMain:
      {
        // Read encoder increments (resets the counter value)
        int16_t currentEncoderIncrements = EncoderButton.GetEncoderIncrements();

        // Will be true, if new encoder position is available
        if (currentEncoderIncrements != 0)
        {
          bool changed = false;

          // Increment or decrement current setting value
          switch (_dashboardLiquid)
          {
            case eLiquid1:
              _dashboardLiquid = currentEncoderIncrements > 0 ? eLiquid2 : eLiquid1;
              changed = currentEncoderIncrements > 0;
              break;
            case eLiquid2:
              _dashboardLiquid = currentEncoderIncrements > 0 ? eLiquid3 : eLiquid1;
              changed = true;
              break;
            case eLiquid3:
              _dashboardLiquid = currentEncoderIncrements > 0 ? eLiquid3 : eLiquid2;
              changed = currentEncoderIncrements < 0;
              break;
            default:
              break;
          }

          if (changed)
          {
            // Short beep sound
            tone(_pinBuzzer, 500, 40);

            // Update display and pump values
            UpdateValues();
            
            // Draw checkboxes
            Display.DrawCheckBoxes(_dashboardLiquid, true);
            
            // Debounce settings change
            delay(200);
          }
        }

#if defined(WIFI_MIXER)
        // Draw wifi icons
        Display.DrawWifiIcons();
#endif

        // Check for button press
        if (EncoderButton.IsButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 500, 40);

          // Incrementing the setting value taking into account the overflow
          _dashboardLiquid = _dashboardLiquid + 1 >= (MixtureLiquid)MixtureLiquidDashboardMax ? eLiquid1 : (MixtureLiquid)(_dashboardLiquid + 1);

          // Update all values
          UpdateValues();
          
          // Draw checkboxes
          Display.DrawCheckBoxes(_dashboardLiquid, true);
          
          // Debounce settings change
          delay(200);
        }

        // Check for long button press
        if (EncoderButton.IsLongButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 800, 40);

          // Exit dashboard and enter menu mode
          Execute(eExit);
          _currentState = eMenu;
          _currentMenuState = eDashboard;
          Execute(eEntry);
          return;
        }

        // Check for screen saver timeout
        if (millis() - EncoderButton.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS &&
          millis() - Pumps.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS)
        {
          // Exit dashboard mode and enter screen saver mode
          Execute(eExit);
          _lastState = eDashboard;
          _currentState = eScreenSaver;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Function cleaning state
//===============================================================
void StateMachine::FctCleaning(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show cleaning page
        Serial.println("[MAIN] Enter Cleaning Mode");
        Display.ShowCleaningPage();

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        EncoderButton.IsButtonPress();
        EncoderButton.IsLongButtonPress();
      }
      break;
    case eMain:
      {
        // Check for button press
        if (EncoderButton.IsButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 500, 40);
          
          // Incrementing the setting value taking into account the overflow
          _cleaningLiquid = _cleaningLiquid + 1 >= (MixtureLiquid)MixtureLiquidCleaningMax ? eLiquid1 : (MixtureLiquid)(_cleaningLiquid + 1);

          // Update display and pump values
          UpdateValues();

          // Draw checkboxes
          Display.DrawCheckBoxes(_cleaningLiquid);

          // Debounce settings change
          delay(200);
        }

#if defined(WIFI_MIXER)
        // Draw wifi icons
        Display.DrawWifiIcons();
#endif

        // Check for long button press
        if (EncoderButton.IsLongButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 800, 40);

          // Exit cleaning mode and return to menu mode
          Execute(eExit);
          _currentState = eMenu;
          _currentMenuState = eCleaning;
          Execute(eEntry);
          return;
        }
        
        // Check for screen saver timeout
        if (millis() - EncoderButton.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS &&
          millis() - Pumps.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS)
        {
          // Exit cleaning mode and enter screen saver mode
          Execute(eExit);
          _lastState = eCleaning;
          _currentState = eScreenSaver;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Function reset state
//===============================================================
void StateMachine::FctReset(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {        
        // Update all values
        UpdateValues();
        
        // Draw reset info box over current page
        Serial.println("[MAIN] Enter Reset Mode");
        Display.DrawInfoBox("Mixture", "reset!");

        // Save reset page start time
        _resetTimestamp = millis();

        // Long beep sound
        tone(_pinBuzzer, 800, 500);
      }
      break;
    case eMain:
      {
        // Wait for the reset page display time
        if ((millis() - _resetTimestamp) > ResetTime_ms)
        {
          // Exit reset mode and return to dashboard mode
          Execute(eExit);
          _currentState = eDashboard;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Function settings state
//===============================================================
void StateMachine::FctSettings(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show settings page
        Serial.println("[MAIN] Enter Settings Mode");
        Display.ShowSettingsPage();
        
        // Debounce page change
        delay(500);

        // Reset and ignore user input
        EncoderButton.GetEncoderIncrements();
        EncoderButton.IsLongButtonPress(); 
        EncoderButton.IsButtonPress();
      }
      break;
    case eMain:
      {
#if defined(WIFI_MIXER)
        // Check for short button press
        if (EncoderButton.IsButtonPress())
        {
          // Update wifi mode
          Wifihandler.SetWifiMode(Wifihandler.GetWifiMode() == WIFI_MODE_AP ? WIFI_MODE_NULL : WIFI_MODE_AP);

          // Short beep sound
          tone(_pinBuzzer, 500, 40);

          // Draw settings in partial update mode
          Display.DrawWifiIcons(true);
          Display.DrawSettings();
        }

        // Draw wifi icons
        Display.DrawWifiIcons();
#endif

        // Check for long button press
        if (EncoderButton.IsLongButtonPress())
        {
          // Short beep sound
          tone(_pinBuzzer, 800, 40);

          // Exit cleaning mode and return to menu mode
          Execute(eExit);
          _currentState = eMenu;
          _currentMenuState = eSettings;
          Execute(eEntry);
          return;
        }

        // Check for screen saver timeout
        if (millis() - EncoderButton.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS &&
          millis() - Pumps.GetLastUserAction() > SCREENSAVER_TIMEOUT_MS)
        {
          // Exit settings mode and enter screen saver mode
          Execute(eExit);
          _lastState = eSettings;
          _currentState = eScreenSaver;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
      {
#if defined(WIFI_MIXER)
        Wifihandler.Save();
#endif
      }
      break;
    default:
      break;
  }
}

//===============================================================
// Function screen saver state
//===============================================================
void StateMachine::FctScreenSaver(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();
        
        // Show page
        Serial.println("[MAIN] Enter Screen Saver Mode");
        Display.ShowScreenSaverPage();
        
        // Reset and ignore user input
        EncoderButton.GetEncoderIncrements();
        EncoderButton.IsLongButtonPress(); 
        EncoderButton.IsButtonPress();
      }
      break;
    case eMain:
      {
        // Draw screen saver
        Display.DrawScreenSaver();
        
        // Check for user input
        if (EncoderButton.GetEncoderIncrements() != 0 ||
          EncoderButton.IsLongButtonPress() ||
          EncoderButton.IsButtonPress() ||
          Pumps.IsEnabled())
        {
          // Exit screen saver mode and return to last mode
          Execute(eExit);
          _currentState = _lastState;
          Execute(eEntry);
          return;
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Updates all values in display, pumps driver and wifi
//===============================================================
void StateMachine::UpdateValues(uint32_t clientID)
{
  // Update display driver
  Display.SetMenuState(_currentMenuState);
  Display.SetDashboardLiquid(_dashboardLiquid);
  Display.SetCleaningLiquid(_cleaningLiquid);
  
  // Update pump driver
  switch (_currentState)
  {
    case eDashboard:
      {
        bool dashboardLiquid1 = _dashboardLiquid == eLiquid1;
        bool dashboardLiquid2 = _dashboardLiquid == eLiquid2;
        bool dashboardLiquid3 = _dashboardLiquid == eLiquid3;
        Pumps.SetPumps(dashboardLiquid1, dashboardLiquid2, dashboardLiquid3);
      }
      break;
    case eCleaning:
      {
        bool cleaningLiquid1 = _cleaningLiquid == eLiquidAll || _cleaningLiquid == eLiquid1;
        bool cleaningLiquid2 = _cleaningLiquid == eLiquidAll || _cleaningLiquid == eLiquid2;
        bool cleaningLiquid3 = _cleaningLiquid == eLiquidAll || _cleaningLiquid == eLiquid3;
        Pumps.SetPumps(cleaningLiquid1, cleaningLiquid2, cleaningLiquid3);
      }
      break;
    default:
    case eMenu:
    case eReset:
    case eSettings:
      {
        Pumps.SetPumps(false, false, false); // zero
      }
      break;
  }
}
