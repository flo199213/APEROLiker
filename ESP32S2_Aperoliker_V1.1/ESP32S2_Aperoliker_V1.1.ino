/**
 * Aperoliker V1.1
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 * 
 * ==============================================================
 * 
 * Configuration Wemos S2 Mini:
 * - Board: "ESP32S2 Dev Module"
 * - CPU Frequency: "240MHz (WiFi)"
 * - USB CDC On Boot: "Enabled"   <------------ Important!
 * - USB DFU On Boot: "Disabled"
 * - USB Firmware MSC On Boot: "Disabled"
 * - Flash Size: "4Mb (32Mb)"
 * - PSRAM: "Enabled"
 * - Upload Mode: "Internal USB"
 * - Upload Speed: "921600"
 * 
 * -> Leave everything else on default!
 * 
 * Important notice:
 * If the Wemos S2 Mini is programmed via the Arduino IDE, the
 * "USB CDC On Boot" flag must be set at all times. This flag
 * causes the Wemos S2 Mini to report as a COM interface immediately
 * after booting via USB. This means that the microcontroller can
 * be programmed via the Arduino Ide WITHOUT having to press the 
 * "BOOT" and "RESET" buttons again. (This allows the housing of
 * the Aperoliker/Hugoliker to be closed).
 * 
 * ==============================================================
 */

//===============================================================
// Includes
//===============================================================
#include "Config.h"
#include <SPI.h>
#include <Adafruit_ST7789.h>
#include "AngleHelper.h"
#include "EncoderButtonDriver.h"
#include "PumpDriver.h"
#include "DisplayDriver.h"

//===============================================================
// Defines
//===============================================================
// Rotary encoder pin defines
#define PIN_ENCODER_OUTA        8   // GPIO 8  -> rotary encoder output A
#define PIN_ENCODER_OUTB        11  // GPIO 11 -> rotary encoder output B
#define PIN_ENCODER_BUTTON      10  // GPIO 10 -> rotary encoder button

// Pumps pin defines
#define PIN_PUMP_1              1   // GPIO 1  -> pump 1 power
#define PIN_PUMP_2              2   // GPIO 2  -> pump 2 power
#define PIN_PUMP_3              4   // GPIO 4  -> pump 3 power
#define PIN_PUMPS_ENABLE        12  // GPIO 12 -> dispensing lever pumps enable, input
#define PIN_PUMPS_ENABLE_GND    13  // GPIO 13 -> dispensing lever pumps enable, GND

// Display pin defines
#define PIN_TFT_DC              37  // GPIO 37 -> TFT data/command
#define PIN_TFT_RST             38  // GPIO 38 -> TFT reset
#define PIN_TFT_CS              34  // GPIO 34 -> TFT chip select (Not connected! ST7789 is always selected)
#define PIN_TFT_SDA             35  // GPIO 35 -> TFT serial data input/output
#define PIN_TFT_SCL             36  // PGIO 36 -> TFT serial clock

// Buzzer pin defines
#define PIN_BUZZER              17  // GPIO 17 -> piezo buzzer

// LED pin defines
#define PIN_LEDLIGHT            6   // GPIO 6  -> power LEDs

// Angle handling defines
#define STEPANGLE_DEGREES       3   // Angle which will be used for one encoder step
#define MINANGLE_DEGREES        6   // Minimum distance angle between two angle settings

//===============================================================
// Global variables
//===============================================================
PumpDriver *pumps = NULL;
EncoderButtonDriver *encoderButton = NULL;
Adafruit_ST7789 *tft = NULL;
DisplayDriver *display = NULL;

// Timer variables for alive counter
uint32_t aliveTimestamp = 0;
const uint32_t AliveTime_ms = 2000;

// Timer variables for blink counter
uint32_t blinkTimestamp = 0;
const uint32_t BlinkTime_ms = 100;

// Timer variables for reset counter
uint32_t resetTimestamp = 0;
const uint32_t ResetTime_ms = 2000;

// Mixture settings
MixtureSetting currentSetting = eLiquid1;
int16_t liquid1Angle_Degrees = 0;
int16_t liquid2Angle_Degrees = 0;
int16_t liquid3Angle_Degrees = 0;
double liquid1_Percentage = 0;
double liquid2_Percentage = 0;
double liquid3_Percentage = 0;

// Cleaning mode settings
bool cleaningAll = true;

// Manual mode settings
double liquid1Manual_Percentage = 0;
double liquid2Manual_Percentage = 0;
double liquid3Manual_Percentage = 0;

// State machine
MixerState currentState = eDashboard;
MixerState currentMenuState = eDashboard;

// Main task
TaskHandle_t mainTaskHandle = NULL;
TaskHandle_t timerTaskHandle = NULL;

//===============================================================
// Interrupt on pumps enable changing state
//===============================================================
void IRAM_ATTR ISR_Pumps_Enable()
{
  // If rising edge (button was released) -> disable pumps
  // If falling edge (button was pressed) -> enable pumps
  digitalRead(PIN_PUMPS_ENABLE) ? pumps->Disable() : pumps->Enable();
}

//===============================================================
// Interrupt on button changing state
//===============================================================
void IRAM_ATTR ISR_EncoderButton()
{
  encoderButton->ButtonEvent();
}

//===============================================================
// Interrupt on encoder out A
//===============================================================
void IRAM_ATTR ISR_EncoderA()
{
  encoderButton->DoEncoderA();
}

//===============================================================
// Interrupt on encoder out B
//===============================================================
void IRAM_ATTR ISR_EncoderB()
{
  encoderButton->DoEncoderB();
}

//===============================================================
// Setup function
//===============================================================
void setup(void)
{
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("[SETUP] Aperoliker V1.1");

  // Initialize SPI
  SPIClass *spi = new SPIClass(HSPI);
  spi->begin(PIN_TFT_SCL, -1, PIN_TFT_SDA, PIN_TFT_CS);

  // Initialize display
  tft = new Adafruit_ST7789(spi, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
  display = new DisplayDriver(tft);
  display->Begin();

  // Initialize GPIOs
  pinMode(PIN_PUMPS_ENABLE, INPUT_PULLUP);
  pinMode(PIN_PUMPS_ENABLE_GND, OUTPUT);
  pinMode(PIN_LEDLIGHT, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize outputs
  digitalWrite(PIN_PUMPS_ENABLE_GND, LOW); // Fixed GND value (0V, LOW)
  digitalWrite(PIN_LEDLIGHT, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Initialize encoder button
  encoderButton = new EncoderButtonDriver(PIN_ENCODER_OUTA, PIN_ENCODER_OUTB, PIN_ENCODER_BUTTON);
  encoderButton->Begin();
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTA), ISR_EncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTB), ISR_EncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_BUTTON), ISR_EncoderButton, CHANGE);
  
  // Initialize pumps driver
  pumps = new PumpDriver(PIN_PUMP_1, PIN_PUMP_2, PIN_PUMP_3);
  pumps->Begin();
  
  // Show intro page
  display->ShowIntroPage();
  delay(3000);
  
  // Set Defaults
  SetMixtureDefaults();
  
  // Allow interrupts for encoder button
  sei();

  // Show help page until button is pressed
  display->ShowHelpPage();
  bool infoBoxShown = false;
  while (!encoderButton->IsButtonPress())
  {
    // If someone turns the knob, not knowing what to do, display the help text
    if (!infoBoxShown && encoderButton->GetEncoderIncrements() > 0)
    {
      // Draw info box with help text
      display->DrawInfoBox("Press Button", "to start!");
      infoBoxShown = true;
    }
    delay(1);
  }
  
  // Short beep sound
  tone(PIN_BUZZER, 500, 40);

  // Call Statemachine with entry event
  StateMachine(eEntry);

  // Initialize interrupt for dispenser lever
  attachInterrupt(digitalPinToInterrupt(PIN_PUMPS_ENABLE), ISR_Pumps_Enable, CHANGE);

  // Start main task
  xTaskCreate(Main_Task, "Main_Task", 4096, NULL, 10, &mainTaskHandle);

  // Final debug output
  Serial.println("[SETUP]");
  Serial.println("[SETUP] ESP32S2 sketch size: " + String(ESP.getSketchSize()) + " bytes");
  Serial.println("[SETUP] ESP32S2 free sketch space: " + String(ESP.getFreeSketchSpace()) + " bytes");
  Serial.println("[SETUP] ESP32S2 SDK version: " + String(ESP.getSdkVersion()));
  Serial.println("[SETUP] ESP32S2 CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
  Serial.println("[SETUP] Finished");
}

//===============================================================
// Loop function
//===============================================================
void loop()
{
  // Show debug alive message
  if ((millis() - aliveTimestamp) > AliveTime_ms)
  {
    aliveTimestamp = millis();
    Serial.println("[LOOP] Alive");
    
    // Print mixture to serial port
    PrintMixture();
  }

  // Flash LED light if dispensing is in progress
  if (pumps->IsEnabled() && currentState != eMenu)
  {
    // Set LED to blink
    if ((millis() - blinkTimestamp) > BlinkTime_ms)
    {
      blinkTimestamp = millis();
      digitalWrite(PIN_LEDLIGHT, !digitalRead(PIN_LEDLIGHT));
    }
  }
  else
  {
    // Set LED to on
    digitalWrite(PIN_LEDLIGHT, HIGH);
  }

  // Update pump outputs
  pumps->UpdateOutputs();
}

//===============================================================
// Main task function
//===============================================================
void Main_Task(void *arg)
{
  while(1)
  {
    // Run statemachine with main task event
    StateMachine(eMain);

    // Execution time for the other tasks
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

//===============================================================
// General state machine function
//===============================================================
void StateMachine(MixerEvent event)
{
  switch (currentState)
  {
    case eMenu:
      FctMenu(event);
      break;
    case eCleaning:
      FctCleaning(event);
      break;
    case eManual:
      FctManual(event);
      break;
    case eReset:
      FctReset(event);
      break;
    default:
    case eDashboard:
      FctDashboard(event);
      break;
  }
}

//===============================================================
// Function menu state
//===============================================================
void FctMenu(MixerEvent event)
{
  switch (event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show menu page
        Serial.println("[MAIN] Enter Menu Mode");
        display->ShowMenuPage(currentMenuState);

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        encoderButton->GetEncoderIncrements();
        encoderButton->IsButtonPress();
      }
      break;
    case eMain:
      {
        // Read encoder increments (resets the counter value)
        int16_t currentEncoderIncrements = encoderButton->GetEncoderIncrements();

        // Check for changed encoder value
        if (currentEncoderIncrements != 0)
        {
          // Increment or decrement current menu state
          switch (currentMenuState)
          {
            case eDashboard:
              currentMenuState = currentEncoderIncrements > 0 ? eDashboard : eCleaning;
              break;
            case eCleaning:
              currentMenuState = currentEncoderIncrements > 0 ? eDashboard : eManual;
              break;
            case eManual:
              currentMenuState = currentEncoderIncrements > 0 ? eCleaning : eReset;
              break;
            case eReset:
              currentMenuState = currentEncoderIncrements > 0 ? eManual : eReset;
              break;
            default:
              break;
          }

          // Update menu
          display->DrawMenu(currentMenuState, false);
        }

        // Check for button press
        if (encoderButton->IsButtonPress())
        {
          if (currentMenuState != eReset)
          {
            // Short beep sound
            tone(PIN_BUZZER, 500, 40);
          }
          
          // Exit dashboard and enter new selected mode
          StateMachine(eExit);
          currentState = currentMenuState;
          StateMachine(eEntry);
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
void FctDashboard(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show dashboard page
        Serial.println("[MAIN] Enter Dashboard Mode");
        display->ShowDashboardPage();

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        encoderButton->GetEncoderIncrements();
        encoderButton->IsLongButtonPress();
        encoderButton->IsButtonPress();
      }
      break;
    case eMain:
      {
        // Read encoder increments (resets the counter value)
        int16_t currentEncoderIncrements = encoderButton->GetEncoderIncrements();

        // Will be true, if new encoder position is available
        if (currentEncoderIncrements != 0)
        {
          // Increment or decrement angle
          switch (currentSetting)
          {
            case eLiquid1:
              IncrementAngle(&liquid1Angle_Degrees, liquid2Angle_Degrees, liquid3Angle_Degrees, currentEncoderIncrements * STEPANGLE_DEGREES);
              break;
            case eLiquid2:
              IncrementAngle(&liquid2Angle_Degrees, liquid3Angle_Degrees, liquid1Angle_Degrees, currentEncoderIncrements * STEPANGLE_DEGREES);
              break;
            case eLiquid3:
              IncrementAngle(&liquid3Angle_Degrees, liquid1Angle_Degrees, liquid2Angle_Degrees, currentEncoderIncrements * STEPANGLE_DEGREES);
              break;
            default:
              break;
          }
          
          // Draw current value string and doughnut chart in partial updating mode
          display->DrawCurrentValues();
          display->DrawDoughnutChart3(currentEncoderIncrements > 0);
        }

        // Check for button press
        if (encoderButton->IsButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 500, 40);

          // Change the currently selected setting
          ChangeSetting();
          
          // Draw legend and doughnut chart in partial updating mode
          display->DrawLegend();
          display->DrawDoughnutChart3(false);
          
          // Debounce settings change
          delay(200);
        }

        // Check for long button press
        if (encoderButton->IsLongButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 800, 40);

          // Exit dashboard and enter menu mode
          StateMachine(eExit);
          currentState = eMenu;
          currentMenuState = eDashboard;
          StateMachine(eEntry);
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
void FctCleaning(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show cleaning page
        Serial.println("[MAIN] Enter Cleaning Mode");
        display->ShowCleaningPage(cleaningAll);

        // Debounce page change
        delay(500);

        // Reset and ignore user input
        encoderButton->IsLongButtonPress();
      }
      break;
    case eMain:
      {
        // Check for button press
        if (encoderButton->IsButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 500, 40);

          if (cleaningAll)
          {
            cleaningAll = false;
            ChangeSetting();
          }
          else
          {
            switch (currentSetting)
            {
              case eLiquid1:
                ChangeSetting();
                break;
              case eLiquid2:
                ChangeSetting();
                break;
              case eLiquid3:
              default:
                cleaningAll = true;
                break;
            }
          }
          
          // Update display and pump values
          UpdateValues();
          display->DrawCheckBoxes(cleaningAll);

          // Debounce settings change
          delay(200);
        }

        // Check for long button press
        if (encoderButton->IsLongButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 800, 40);

          // Exit cleaning mode and return to menu mode
          StateMachine(eExit);
          currentState = eMenu;
          currentMenuState = eCleaning;
          StateMachine(eEntry);
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Function manual state
//===============================================================
void FctManual(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Update display and pump values
        UpdateValues();

        // Show manual page
        Serial.println("[MAIN] Enter Manual Mode");
        display->ShowManualPage();
        
        // Debounce page change
        delay(500);

        // Reset and ignore user input
        encoderButton->GetEncoderIncrements();
        encoderButton->IsLongButtonPress(); 
        encoderButton->IsButtonPress();
      }
      break;
    case eMain:
      {
        // Read encoder increments (resets the counter value)
        int16_t currentEncoderIncrements = encoderButton->GetEncoderIncrements();

        // Will be true, if new encoder position is available
        if (currentEncoderIncrements != 0)
        {
          // Increment or decrement angle
          switch (currentSetting)
          {
            case eLiquid1:
              liquid1Manual_Percentage = max(0.0, min(100.0, liquid1Manual_Percentage + currentEncoderIncrements));
              break;
            case eLiquid2:
              liquid2Manual_Percentage = max(0.0, min(100.0, liquid2Manual_Percentage + currentEncoderIncrements));
              break;
            case eLiquid3:
              liquid3Manual_Percentage = max(0.0, min(100.0, liquid3Manual_Percentage + currentEncoderIncrements));
              break;
            default:
              break;
          }

          // Update values in display and pumps
          UpdateValues();

          // Draw bargraph in partial updating mode
          display->DrawCurrentValues();
          display->DrawBargraph();
        }

        // Check for button press
        if (encoderButton->IsButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 500, 40);

          // Change the currently selected setting
          ChangeSetting();
          
          // Draw bargraph in partial updating mode
          display->DrawBargraph();
          
          // Debounce settings change
          delay(200);
        }

        // Check for long button press
        if (encoderButton->IsLongButtonPress())
        {
          // Short beep sound
          tone(PIN_BUZZER, 800, 40);

          // Exit cleaning mode and return to menu mode
          StateMachine(eExit);
          currentState = eMenu;
          currentMenuState = eManual;
          StateMachine(eEntry);
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
void FctReset(MixerEvent event)
{
  switch(event)
  {
    case eEntry:
      {
        // Draw reset info box over current page
        Serial.println("[MAIN] Enter Reset Mode");
        display->DrawInfoBox("Mixture", "reset!");

        // Reset mixture
        SetMixtureDefaults();

        // Save reset page start time
        resetTimestamp = millis();

        // Long beep sound
        tone(PIN_BUZZER, 800, 500);
      }
      break;
    case eMain:
      {
        // Wait for the reset page display time
        if ((millis() - resetTimestamp) > ResetTime_ms)
        {
          // Exit reset mode and return to dashboard mode
          StateMachine(eExit);
          currentState = eDashboard;
          currentMenuState = eReset;
          StateMachine(eEntry);
        }
      }
      break;
    case eExit:
    default:
      break;
  }
}

//===============================================================
// Changes the current ingredient setting
//===============================================================
void ChangeSetting()
{
  // Incrementing the setting value taking into account the overflow
  currentSetting = currentSetting + 1 >= (MixtureSetting)MixtureSettingsMax ? eLiquid1 : (MixtureSetting)(currentSetting + 1);

  // Update all values
  UpdateValues();
}

//===============================================================
// Increments the value by the angle distance given
//===============================================================
void IncrementAngle(int16_t* value, int16_t nextBorder, int16_t previousBorder, int16_t angleDistance_Degrees)
{
  bool clockwise = angleDistance_Degrees > 0;
  
  for (uint16_t index = 0; index < abs(angleDistance_Degrees); index++)
  {
    int16_t newValue = Move360(*value, clockwise ? 1 : -1);
    int16_t distanceToNextBorder_Degrees = GetDistanceDegrees(newValue, clockwise ? nextBorder : previousBorder);
    distanceToNextBorder_Degrees = clockwise ? distanceToNextBorder_Degrees : 360 - distanceToNextBorder_Degrees;
    
    if (distanceToNextBorder_Degrees >= MINANGLE_DEGREES)
    {
      *value = newValue;
    }
    else
    {
      break;
    }
  }
  
  // Update all values
  UpdateValues();
}

//===============================================================
// Resets the mixture to default recipe
//===============================================================
void SetMixtureDefaults()
{ 
  // Set mixture to:
  // Aperol: 34%, Soda: 16%, Prosecco: 50% (Official Aperol recipe)
  liquid1Angle_Degrees = 0;                                                 // 33,33%
  liquid2Angle_Degrees = liquid1Angle_Degrees + 120;                        // 15,83%
  liquid3Angle_Degrees = liquid1Angle_Degrees + liquid2Angle_Degrees + 57;  // 50,84%

  // Update all values
  UpdateValues();
}

//===============================================================
// Updates all values in display, pumps driver and prints it to 
// serial port
//===============================================================
void UpdateValues()
{
  int16_t liquid1Distance_Degrees = GetDistanceDegrees(liquid1Angle_Degrees, liquid2Angle_Degrees);
  int16_t liquid2Distance_Degrees = GetDistanceDegrees(liquid2Angle_Degrees, liquid3Angle_Degrees);
  int16_t liquid3Distance_Degrees = GetDistanceDegrees(liquid3Angle_Degrees, liquid1Angle_Degrees);

  // Calculate percentage values
  liquid1_Percentage = (double)liquid1Distance_Degrees * 100.0 / 360.0;
  liquid2_Percentage = (double)liquid2Distance_Degrees * 100.0 / 360.0;
  liquid3_Percentage = (double)liquid3Distance_Degrees * 100.0 / 360.0;

  // Update display
  display->UpdateSetting(currentSetting);
  display->UpdateAngles(liquid1Angle_Degrees, liquid2Angle_Degrees, liquid3Angle_Degrees);
  
  // Update display and pump driver
  switch (currentState)
  {
    case eDashboard:
      display->UpdatePercentages(liquid1_Percentage, liquid2_Percentage, liquid3_Percentage);
      pumps->UpdatePercentages(liquid1_Percentage, liquid2_Percentage, liquid3_Percentage, true);
      break;
    case eCleaning:
      {
        double cleaning1_Percentage = cleaningAll || currentSetting == eLiquid1 ? 100.0 : 0.0; // max value (100%) or min value (0%)
        double cleaning2_Percentage = cleaningAll || currentSetting == eLiquid2 ? 100.0 : 0.0; // max value (100%) or min value (0%)
        double cleaning3_Percentage = cleaningAll || currentSetting == eLiquid3 ? 100.0 : 0.0; // max value (100%) or min value (0%)
        pumps->UpdatePercentages(cleaning1_Percentage, cleaning2_Percentage, cleaning3_Percentage, false);
      }
      break;
    case eManual:
      display->UpdatePercentages(liquid1Manual_Percentage, liquid2Manual_Percentage, liquid3Manual_Percentage);
      pumps->UpdatePercentages(liquid1Manual_Percentage, liquid2Manual_Percentage, liquid3Manual_Percentage, false);
      break;
    case eReset:
      break;
    default:
    case eMenu:
      pumps->UpdatePercentages(0, 0, 0, false); // zero (0%)
      break;
  }
}

//===============================================================
// Prints current mixture to serial port
//===============================================================
void PrintMixture()
{
  // Calculate sum
  double sum_Percentage = liquid1_Percentage + liquid2_Percentage + liquid3_Percentage;
  
  // Serial output
  Serial.print("[PRINT] ");
  Serial.print(liquid1String);
  Serial.print(": ");
  Serial.print(liquid1_Percentage);
  Serial.print("% (");
  Serial.print(liquid1Angle_Degrees);
  Serial.print("°), ");
  Serial.print(liquid2String);
  Serial.print(": ");
  Serial.print(liquid2_Percentage);
  Serial.print("% (");
  Serial.print(liquid2Angle_Degrees);
  Serial.print("°), ");
  Serial.print(liquid3String);
  Serial.print(": ");
  Serial.print(liquid3_Percentage);
  Serial.print("% (");
  Serial.print(liquid3Angle_Degrees);
  Serial.print("°), Sum: ");
  Serial.print(sum_Percentage);
  
  if ((sum_Percentage - 100.0) > 0.1 || (sum_Percentage - 100.0) < -0.1)
  {
    // Percentage error
    Serial.println("% Error: Sum of all percentages must be ~100%");
  }
  else
  {
    Serial.println("%");
  }
}
