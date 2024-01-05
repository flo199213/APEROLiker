/**
 * Includes all display functions
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
 */
 
#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

//===============================================================
// Includes
//===============================================================
#include "Config.h"
#include <Arduino.h>
#include <SPI.h>
#include <SPIFFS.h>
#include "SPIFFS_ImageReader.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans9pt7b.h>
#include "AngleHelper.h"


//===============================================================
// Defines
//===============================================================
#define TFT_DEG2RAD             0.017453292519943295769236907684886F
#define TFT_WIDTH               240
#define TFT_HEIGHT              240

#define HEADEROFFSET_Y          30
#define HEADER_MARGIN           10
#define X0_DOUGHNUTCHART        65
#define Y0_DOUGHNUTCHART        130
#define R_INNER_DOUGHNUTCHART   30
#define R_OUTER_DOUGHNUTCHART   60
#define X_LEGEND                (X0_DOUGHNUTCHART + R_OUTER_DOUGHNUTCHART + 10)
#define Y_LEGEND                (Y0_DOUGHNUTCHART - R_OUTER_DOUGHNUTCHART)
#define WIDTH_LEGEND            (TFT_WIDTH - X_LEGEND - 10)
#define HEIGHT_LEGEND           (TFT_HEIGHT - Y_LEGEND - 10)

#define INFOBOX_MARGIN_VERT     60
#define INFOBOX_MARGIN_HORI     25
#define INFOBOX_CORNERRADIUS    8

#define SHORTLINEOFFSET         20
#define LONGLINEOFFSET          30
#define LOONGLINEOFFSET         50
#define SPACERANGLE_DEGREES     1  // Angle which will be displayed as spacer between pie elements (will be multiplied by 2, left and right of the setting angle)


//===============================================================
// Enums
//===============================================================
enum MixtureSetting : uint16_t
{
  eLiquid1 = 0,
  eLiquid2 = 1,
  eLiquid3 = 2
};
const int MixtureSettingsMax = 3;


//===============================================================
// Class for handling display functions
//===============================================================
class DisplayDriver
{
  public:
    // Constructor
    DisplayDriver(Adafruit_ST7789* tft);

    // Initializes the display driver
    void Begin();

    // Updates the setting value
    void UpdateSetting(MixtureSetting currentSetting);

    // Updates the angles values
    void UpdateAngles(int16_t liquid1Angle_Degrees, int16_t liquid2Angle_Degrees, int16_t liquid3Angle_Degrees);

    // Updates the percentage values
    void UpdatePercentages(double liquid1_Percentage, double liquid2_Percentage, double liquid3_Percentage);
    
    // Draws intro page
    void ShowIntroPage();
    
    // Draws cleaning page
    void ShowCleaningPage();
    
    // Draws help page
    void ShowHelpPage();
    
    // Draws settings page
    void ShowSettingsPage();
    
    // Draws the info box
    void DrawInfoBox(const String &line1, const String &line2);
    
    // Draws the legend
    void DrawLegend();
    
    // Draws current values
    void DrawCurrentValues(bool isfullUpdate = false);

    // Draws full doughnut chart
    void DrawDoughnutChart3();
    
    // Draws doughnut chart
    void DrawDoughnutChart3(bool clockwise, bool isfullUpdate = false);

  private:
    Adafruit_ST7789* _tft;
    SPIFFS_ImageReader reader;

    bool _spiffsAvailable = false;

    // Mixture Settings
    MixtureSetting _currentSetting = eLiquid1;
    int16_t _liquid1Angle_Degrees = 0;
    int16_t _liquid2Angle_Degrees = 0;
    int16_t _liquid3Angle_Degrees = 0;
    double _liquid1_Percentage;
    double _liquid2_Percentage;
    double _liquid3_Percentage;

    char output[30];
        
    // Last draw values
    String _lastDraw_Liquid1String = "";
    String _lastDraw_Liquid2String = "";
    String _lastDraw_Liquid3String = "";
    int16_t _lastDraw_liquid1Angle_Degrees = 0;
    int16_t _lastDraw_liquid2Angle_Degrees = 0;
    int16_t _lastDraw_liquid3Angle_Degrees = 0;

    // Draws default header Text
    void DrawHeader();
    
    // Draws header Text
    void DrawHeader(const String &text);
    
    // Only draws partial update of arcs
    void DrawPartial(int16_t angle, int16_t lastAngle,  uint16_t colorAfter, uint16_t colorBefore, bool clockwise);
    
    // Draw an arc with a defined thickness
    void FillArc(int16_t start_angle, int16_t distance_Degrees, uint16_t color);
    
    // Draws a string centered
    void DrawCenteredString(const String &text, int16_t x, int16_t y, bool underlined, uint16_t lineColor);
    
    // Formats double value
    String FormatValue(double value, int mainPlaces, int decimalPlaces);
};
#endif
    
