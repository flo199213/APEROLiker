/**
 * Aperoliker main file
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
 * - Partition Scheme: "No OTA (1MB APP/3MB SPIFFS)"
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
#include <Arduino.h>
#include <USB.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Adafruit_ST7789.h>
#include "Config.h"
#include "StateMachine.h"
#include "EncoderButtonDriver.h"
#include "PumpDriver.h"
#include "DisplayDriver.h"
#include "FlowMeterDriver.h"
#include "WifiHandler.h"


//===============================================================
// Defines
//===============================================================
// Rotary encoder pin defines
#define PIN_ENCODER_OUTA        8     // GPIO 8  -> rotary encoder output A
#define PIN_ENCODER_OUTB        11    // GPIO 11 -> rotary encoder output B
#define PIN_ENCODER_BUTTON      10    // GPIO 10 -> rotary encoder button

// Pumps pin defines
#define PIN_PUMP_1              1     // GPIO 1  -> pump 1 power
#define PIN_PUMP_2              2     // GPIO 2  -> pump 2 power
#define PIN_PUMP_3              4     // GPIO 4  -> pump 3 power
#define PIN_PUMPS_ENABLE        12    // GPIO 12 -> dispensing lever pumps enable, input
#define PIN_PUMPS_ENABLE_GND    13    // GPIO 13 -> dispensing lever pumps enable, GND

// Display pin defines
#define PIN_TFT_DC              37    // GPIO 37 -> TFT data/command
#define PIN_TFT_RST             38    // GPIO 38 -> TFT reset
#define PIN_TFT_CS              34    // GPIO 34 -> TFT chip select (Not connected! ST7789 is always selected)
#define PIN_TFT_SDA             35    // GPIO 35 -> TFT serial data input/output
#define PIN_TFT_SCL             36    // PGIO 36 -> TFT serial clock

// Buzzer pin defines
#define PIN_BUZZER              17    // GPIO 17 -> piezo buzzer

// LED pin defines
#define PIN_LEDLIGHT            6     // GPIO 6  -> power LEDs

// Intro defines
#define INTRO_TIME_MS           3000  // Wait for 3 seconds at startup and show intro page


//===============================================================
// Global variables
//===============================================================
Preferences preferences;
Adafruit_ST7789* tft = NULL;

// Timer variables for alive counter
uint32_t aliveTimestamp = 0;
const uint32_t AliveTime_ms = 2000;

// Timer variables for blink counter
uint32_t blinkTimestamp = 0;
const uint32_t BlinkTime_ms = 100;

// Task handles
TaskHandle_t mainTaskHandle = NULL;
TaskHandle_t timerTaskHandle = NULL;

//===============================================================
// Interrupt on pumps enable changing state
//===============================================================
void IRAM_ATTR ISR_Pumps_Enable()
{
  // If rising edge (button was released) -> disable pumps
  // If falling edge (button was pressed) -> enable pumps
  if (digitalRead(PIN_PUMPS_ENABLE))
  {
    // Disable pump power
    Pumps.Disable();

    // Save current flow values to EEPROM
    FlowMeter.Save();
  }
  else
  {
    // Enable pump power
     Pumps.Enable();
  }
}

//===============================================================
// Interrupt on button changing state
//===============================================================
void IRAM_ATTR ISR_EncoderButton()
{
  EncoderButton.ButtonEvent();
}

//===============================================================
// Interrupt on encoder out A
//===============================================================
void IRAM_ATTR ISR_EncoderA()
{
  EncoderButton.DoEncoderA();
}

//===============================================================
// Interrupt on encoder out B
//===============================================================
void IRAM_ATTR ISR_EncoderB()
{
  EncoderButton.DoEncoderB();
}

//===============================================================
// Setup function
//===============================================================
void setup(void)
{
  // Start USB CDC, for the case "USB CDC On Boot" is missing in buid defines
  USBSerial.begin();
  USB.begin();

  // Initialize serial communication
  Serial.begin(115200);
  Serial.println(String("[SETUP] ") + MIXER_NAME + " " + APP_VERSION);

  // Initialize SPIFFS
  bool spiffsAvailable = SPIFFS.begin(true);
  size_t spiffsTotal = SPIFFS.totalBytes();
  size_t spiffsUsed = SPIFFS.usedBytes();
  Serial.println(String("[SETUP] SPIFFS: ") + String(spiffsUsed) + "/" + String(spiffsTotal) + " Bytes used (SPIFFS Available: " + String(spiffsAvailable) + ")");

  // Initialize SPI
  SPIClass* spi = new SPIClass(HSPI);
  spi->begin(PIN_TFT_SCL, -1, PIN_TFT_SDA, PIN_TFT_CS);

  // Initialize display
  tft = new Adafruit_ST7789(spi, PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
  Display.Begin(tft, spiffsAvailable);

  // Show intro page
  Display.ShowIntroPage();
  uint32_t startupTime_ms = millis();

  // Initialize GPIOs
  pinMode(PIN_PUMPS_ENABLE, INPUT_PULLUP);
  pinMode(PIN_PUMPS_ENABLE_GND, OUTPUT);
  pinMode(PIN_LEDLIGHT, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize outputs
  digitalWrite(PIN_PUMPS_ENABLE_GND, LOW); // Fixed GND value (0V, LOW)
  digitalWrite(PIN_LEDLIGHT, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Play startup sound
	for (uint8_t index = 1; index <= 8; index++)
	{
    tone(PIN_BUZZER, index * 100, 65);
    delay(85);
  }

  // Initialize encoder button
  EncoderButton.Begin(PIN_ENCODER_OUTA, PIN_ENCODER_OUTB, PIN_ENCODER_BUTTON);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTA), ISR_EncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_OUTB), ISR_EncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_BUTTON), ISR_EncoderButton, CHANGE);

  // Initialize flow values from EEPROM
  FlowMeter.Load();
  
  // Initialize pump driver
  Pumps.Begin(PIN_PUMP_1, PIN_PUMP_2, PIN_PUMP_3);

  // Initialize state machine
  Statemachine.Begin(PIN_BUZZER);
  
  // Wait for the rest of the intro time
  while ((millis() - startupTime_ms) < INTRO_TIME_MS) { };

  // Allow interrupts for encoder button
  sei();

  // Show help page until button is pressed
  Display.ShowHelpPage();
  bool infoBoxShown = false;
  while (true)
  {
    // If someone turns the knob, not knowing what to do, display the help text
    if (!infoBoxShown && EncoderButton.GetEncoderIncrements() > 0)
    {
      // Draw info box with help text
      Display.DrawInfoBox("Press Button", "to start!");
      infoBoxShown = true;
    }

    // Check for button press user input
    if (EncoderButton.IsButtonPress())
    {
      // Short beep sound
      tone(PIN_BUZZER, 500, 40);

      // Proceed with setup
      break;
    }

    // Contains yield() for ESP32
    delay(1);
  }
  
  // Initialize wifi
  Wifihandler.Begin();

  // Initial run of state machine with entry event
  Statemachine.Execute(eEntry);

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
  Serial.println("[SETUP] ESP32S2 Chip MAC: " + String(WiFi.macAddress()));
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
    Statemachine.PrintMixture();
  }

  // Flash LED light if dispensing is in progress
  if (Pumps.IsEnabled() &&
    (Statemachine.GetCurrentState() == eDashboard ||
    Statemachine.GetCurrentState() == eCleaning))
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
  Pumps.Update();

  // Update wifi, webserver and clients
  Wifihandler.Update();
}

//===============================================================
// Main task function
//===============================================================
void Main_Task(void *arg)
{
  while(1)
  {
    // Run statemachine with main task event
    Statemachine.Execute(eMain);

    // Execution time for the other tasks
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

