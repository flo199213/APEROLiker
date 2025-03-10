/**
 * Aperoliker V1.0
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
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

// Mixture settings
MixtureSetting currentSetting = eLiquid1;
int16_t liquid1Angle_Degrees = 0;
int16_t liquid2Angle_Degrees = 0;
int16_t liquid3Angle_Degrees = 0;
double liquid1_Percentage = 0;
double liquid2_Percentage = 0;
double liquid3_Percentage = 0;

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
  Serial.println("[SETUP] Aperoliker V1.0");

  // Initialize GPIOs
  pinMode(PIN_PUMPS_ENABLE, INPUT_PULLUP);
  pinMode(PIN_PUMPS_ENABLE_GND, OUTPUT);
  pinMode(PIN_LEDLIGHT, OUTPUT);

  // Initialize outputs
  digitalWrite(PIN_PUMPS_ENABLE_GND, LOW); // Fixed GND value (0V, LOW)
  digitalWrite(PIN_LEDLIGHT, LOW);

  // Initialize encoder button
  encoderButton = new EncoderButtonDriver(PIN_ENCODER_OUTA, PIN_ENCODER_OUTB, PIN_ENCODER_BUTTON, PIN_BUZZER);
  encoderButton->Begin();  // <-- Determines cleaning mode state (if button is pressed)
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTA), ISR_EncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTB), ISR_EncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_BUTTON), ISR_EncoderButton, CHANGE);
  
  // Initialize pumps driver
  pumps = new PumpDriver(PIN_PUMP_1, PIN_PUMP_2, PIN_PUMP_3);
  pumps->Begin(encoderButton->IsCleaningMode());
    
  // Initialize SPI
  SPIClass *spi = new SPIClass(HSPI);
  spi->begin(PIN_TFT_SCL, -1, PIN_TFT_SDA, PIN_TFT_CS);

  // Initialize display
  tft = new Adafruit_ST7789(spi, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
  display = new DisplayDriver(tft);
  display->Begin();
  
  // Show intro page
  display->ShowIntroPage();
  delay(3000);
  
  // Set Defaults
  SetMixtureDefaults();
  
  // Allow interrupts for encoder button
  sei();

  // Check for cleaning mode (button pressed => LOW)
  if (encoderButton->IsCleaningMode())
  {
    Serial.println("[SETUP] Enter Cleaning Mode");
    
    // Show cleaning page
    display->ShowCleaningPage();
  }
  else
  {
    Serial.println("[SETUP] Enter Normal Mode");
    
    // Show help page until button is pressed
    display->ShowHelpPage();
    while (!encoderButton->IsButtonPress()) { delay(1); };

    // Show settings page
    display->ShowSettingsPage();
  }
  
  // Initialize interrupt for dispenser lever
  attachInterrupt(digitalPinToInterrupt(PIN_PUMPS_ENABLE), ISR_Pumps_Enable, CHANGE);

  // Start main task
  xTaskCreate(Main_Task, "Main_Task", 4096, NULL, 10, &mainTaskHandle);

  // Final output
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
    // Flash LED light if dispensing is in progress
    if (pumps->IsEnabled())
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

    // Abort loop at this point when cleaning mode is active
    if (encoderButton->IsCleaningMode())
    {
      vTaskDelay(pdMS_TO_TICKS(5));
      continue;
    }
    
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

    // Will be true if button was pressed
    if (encoderButton->IsButtonPress())
    {
      // Change the currently selected setting
      ChangeSetting();
      
      // Draw legend and doughnut chart in partial updating mode
      display->DrawLegend();
      display->DrawDoughnutChart3(false);
    }
    
    // Will be true if button was pressed longer than threshold setting
    if (encoderButton->IsLongButtonPress())
    { 
      // Reset mixture
      SetMixtureDefaults();
      
      // Draw reset info box over current page
      display->DrawInfoBox("Mixture", "reset!");
      delay(2000);
      
      // Show settings page again
      display->ShowSettingsPage();
    }

    vTaskDelay(pdMS_TO_TICKS(5));
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
  display->UpdatePercentages(liquid1_Percentage, liquid2_Percentage, liquid3_Percentage);
  
  // Update pumps driver
  pumps->UpdatePercentages(liquid1_Percentage, liquid2_Percentage, liquid3_Percentage);
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
