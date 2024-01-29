/**
 * Includes all display functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

#include "DisplayDriver.h"

//===============================================================
// Constructor
//===============================================================
DisplayDriver::DisplayDriver(Adafruit_ST7789* tft) :
  _tft(tft)
{
}

//===============================================================
// Initializes the display driver
//===============================================================
void DisplayDriver::Begin()
{
  // Initialize SPIFFS
  _spiffsAvailable = SPIFFS.begin();
  
  // Initialize display
  _tft->init(TFT_WIDTH, TFT_HEIGHT, SPI_MODE3);
  _tft->invertDisplay(true);
  _tft->setRotation(3);
  _tft->setTextWrap(false);
  _tft->setFont(&FreeSans9pt7b);
  _tft->fillScreen(TFT_COLOR_BACKGROUND);

  // Show starting message
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Booting...", TFT_WIDTH / 2, TFT_HEIGHT / 2, false, 0);

  if (_spiffsAvailable)
  {
    // Load startup images to RAM
    _spiffsAvailable &= reader.LoadBMP(startupImageBottle.c_str(), &_imageBottle) == IMAGE_SUCCESS;
    _spiffsAvailable &= reader.LoadBMP(startupImageGlass.c_str(), &_imageGlass) == IMAGE_SUCCESS;
    _spiffsAvailable &= reader.LoadBMP(startupImageLogo.c_str(), &_imageLogo) == IMAGE_SUCCESS;
  }
}

//===============================================================
// Updates the setting value
//===============================================================
void DisplayDriver::UpdateSetting(MixtureSetting currentSetting)
{
  _currentSetting = currentSetting;
}

//===============================================================
// Updates the angles values
//===============================================================
void DisplayDriver::UpdateAngles(int16_t liquid1Angle_Degrees, int16_t liquid2Angle_Degrees, int16_t liquid3Angle_Degrees)
{
  _liquid1Angle_Degrees = liquid1Angle_Degrees;
  _liquid2Angle_Degrees = liquid2Angle_Degrees;
  _liquid3Angle_Degrees = liquid3Angle_Degrees;
}

//===============================================================
// Updates the percentage values
//===============================================================
void DisplayDriver::UpdatePercentages(double liquid1_Percentage, double liquid2_Percentage, double liquid3_Percentage)
{
  _liquid1_Percentage = liquid1_Percentage;
  _liquid2_Percentage = liquid2_Percentage;
  _liquid3_Percentage = liquid3_Percentage;
}

//===============================================================
// Draws intro page
//===============================================================
void DisplayDriver::ShowIntroPage()
{
  // Draw Background
  _tft->fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT * 0.8, TFT_COLOR_STARTPAGE_BACKGROUND);
  _tft->fillRect(0, TFT_HEIGHT * 0.8, TFT_WIDTH, TFT_HEIGHT, TFT_COLOR_STARTPAGE_FOREGROUND);

  if (_spiffsAvailable)
  {
    // Draw startup images
    _imageBottle.Draw(TFT_BOTTLE_POS_X, TFT_BOTTLE_POS_Y, _tft, TFT_TRANSPARENCY_COLOR);
    _imageGlass.Draw(TFT_GLASS_POS_X, TFT_GLASS_POS_Y, _tft, TFT_TRANSPARENCY_COLOR);
    _imageLogo.Draw(TFT_LOGO_POS_X, TFT_LOGO_POS_Y, _tft, TFT_TRANSPARENCY_COLOR);
  }
  else
  {
    // Draw info box (fallback)
    DrawInfoBox("- Startpage -", "NO SPIFFS Files!");
  }
}

//===============================================================
// Draws cleaning page
//===============================================================
void DisplayDriver::ShowCleaningPage(bool cleaningAll)
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Cleaning Mode");

  // Draw checkboxes
  DrawCheckBoxes(cleaningAll);
}

//===============================================================
// Draws help page
//===============================================================
void DisplayDriver::ShowHelpPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Instructions");

  // Fill in help text
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_BODY);

  int marginLeft = 15;
  int marginToHeader = 20;

  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader);
  _tft->print("Short Press:");
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 1 * SHORTLINEOFFSET  + 0 * LONGLINEOFFSET);
  _tft->print(" -> Change Setting");
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 2 * SHORTLINEOFFSET  + 0 * LONGLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(liquid1String);
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 3 * SHORTLINEOFFSET  + 0 * LONGLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(liquid2String);
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 4 * SHORTLINEOFFSET  + 0 * LONGLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(liquid3String);
  
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 4 * SHORTLINEOFFSET  + 1 * LONGLINEOFFSET);
  _tft->print("Rotate:");
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 5 * SHORTLINEOFFSET  + 1 * LONGLINEOFFSET);
  _tft->print(" -> Change Value");

  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 5 * SHORTLINEOFFSET  + 2 * LONGLINEOFFSET);
  _tft->print("Long Press:");
  _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 6 * SHORTLINEOFFSET  + 2 * LONGLINEOFFSET);  
  _tft->print(" -> Menu/Go Back");
}

//===============================================================
// Draws dashboard page
//===============================================================
void DisplayDriver::ShowDashboardPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader();
  
  // Draw chart in first draw mode
  DrawDoughnutChart3();

  // Draw legend
  DrawLegend();

  // Draw current value string
  DrawCurrentValues(true);

  // Draw enjoy message
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Enjoy it!", X0_DOUGHNUTCHART, TFT_HEIGHT - 30, false, 0);
}

//===============================================================
// Draws menu page
//===============================================================
void DisplayDriver::ShowMenuPage(MixerState menuState)
{
    // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Menu");

  // Draw menu
  DrawMenu(menuState, true);
}

//===============================================================
// Draws manual page
//===============================================================
void DisplayDriver::ShowManualPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader();

  // Draw current value string
  DrawCurrentValues(true);

  // Draw bargraph
  DrawBargraph(true);
}

//===============================================================
// Draws default header Text
//===============================================================
void DisplayDriver::DrawHeader()
{
  DrawHeader(defaultHeader);
}

//===============================================================
// Draws header Text
//===============================================================
void DisplayDriver::DrawHeader(const String &text)
{
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_HEADER);
  DrawCenteredString(text, TFT_WIDTH / 2, HEADEROFFSET_Y / 2, false, 0);
  _tft->drawLine(HEADER_MARGIN, HEADEROFFSET_Y, TFT_WIDTH - (2 * HEADER_MARGIN), HEADEROFFSET_Y, TFT_COLOR_FOREGROUND);
}

//===============================================================
// Draws the info box
//===============================================================
void DisplayDriver::DrawInfoBox(const String &line1, const String &line2)
{
  int x = INFOBOX_MARGIN_HORI;
  int y = HEADEROFFSET_Y + INFOBOX_MARGIN_VERT;
  int width = TFT_WIDTH - 2 * INFOBOX_MARGIN_HORI;
  int height = TFT_HEIGHT - HEADEROFFSET_Y - 2 * INFOBOX_MARGIN_VERT;

  // Draw rectangle with colored border
  _tft->fillRoundRect(x, y, width, height, INFOBOX_CORNERRADIUS, TFT_COLOR_INFOBOX_BORDER);
  _tft->fillRoundRect(x + 2, y + 2,  width - 4, height - 4, INFOBOX_CORNERRADIUS, TFT_COLOR_INFOBOX_BACKGROUND);
  
  // Fill in info text
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_INFOBOX_FOREGROUND);
  DrawCenteredString(line1, x + width / 2, y + (height / 2) - (SHORTLINEOFFSET / 2), false, 0);
  DrawCenteredString(line2, x + width / 2, y + (height / 2) + (SHORTLINEOFFSET / 2), false, 0);
}

//===============================================================
// Draws the menu
//===============================================================
void DisplayDriver::DrawMenu(MixerState menuState, bool isfullUpdate)
{
  int16_t marginLeft = MENU_MARGIN_HORI + MENU_MARGIN_ICON;
  int16_t marginToHeader = 40;

  if (isfullUpdate)
  {
    // Draw icons
    _tft->drawXBitmap(MENU_MARGIN_HORI + 8, HEADEROFFSET_Y + marginToHeader + 0 * MENU_LINEOFFSET - 22, icon_dashboard, 32, 32, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(MENU_MARGIN_HORI + 8, HEADEROFFSET_Y + marginToHeader + 1 * MENU_LINEOFFSET - 22, icon_cleaning,  32, 32, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(MENU_MARGIN_HORI + 8, HEADEROFFSET_Y + marginToHeader + 2 * MENU_LINEOFFSET - 22, icon_manual,    32, 32, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(MENU_MARGIN_HORI + 8, HEADEROFFSET_Y + marginToHeader + 3 * MENU_LINEOFFSET - 22, icon_reset,     32, 32, TFT_COLOR_FOREGROUND);

    // Set text properties
    _tft->setTextSize(1);
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);

    _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 0 * MENU_LINEOFFSET);
    _tft->print("Dashboard");
    _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 1 * MENU_LINEOFFSET);
    _tft->print("Cleaning Mode");
    _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 2 * MENU_LINEOFFSET);
    _tft->print("Manual Mode");
    _tft->setCursor(marginLeft, HEADEROFFSET_Y + marginToHeader + 3 * MENU_LINEOFFSET);
    _tft->print("Reset Mixture");
  }

  if (_lastDrawMenuState != menuState || isfullUpdate)
  {
    // Determine position for selected menu state
    int16_t x = MENU_MARGIN_HORI - 2;
    int16_t w = TFT_WIDTH - 2 * MENU_MARGIN_HORI;
    int16_t yOld = HEADEROFFSET_Y + marginToHeader + ((uint16_t)_lastDrawMenuState - 1) * MENU_LINEOFFSET - 6 - MENU_SELECTOR_HEIGHT / 2;
    int16_t yNew = HEADEROFFSET_Y + marginToHeader + ((uint16_t)menuState          - 1) * MENU_LINEOFFSET - 6 - MENU_SELECTOR_HEIGHT / 2;

    // Reset old menu selection on display
    _tft->drawRoundRect(x, yOld, w, MENU_SELECTOR_HEIGHT, MENU_SELECTOR_CORNERRADIUS, TFT_COLOR_BACKGROUND);
    
    // Draw new menu selection on display
    _tft->drawRoundRect(x, yNew, w, MENU_SELECTOR_HEIGHT, MENU_SELECTOR_CORNERRADIUS, TFT_COLOR_MENU_SELECTOR);

    // Save last state
    _lastDrawMenuState = menuState;
  }
}

//===============================================================
// Draw checkboxes
//===============================================================
void DisplayDriver::DrawCheckBoxes(bool cleaningAll)
{
  uint16_t boxWidth = 30;

  // Print selection text
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Select pumps for cleaning:", TFT_WIDTH / 2, TFT_HEIGHT / 3, false, 0);

  // Draw checkboxes
  _tft->drawRect(TFT_WIDTH * 1 / 7, HEADEROFFSET_Y + TFT_HEIGHT / 3, boxWidth, boxWidth, TFT_COLOR_FOREGROUND);
  _tft->drawRect(TFT_WIDTH * 3 / 7, HEADEROFFSET_Y + TFT_HEIGHT / 3, boxWidth, boxWidth, TFT_COLOR_FOREGROUND);
  _tft->drawRect(TFT_WIDTH * 5 / 7, HEADEROFFSET_Y + TFT_HEIGHT / 3, boxWidth, boxWidth, TFT_COLOR_FOREGROUND);
  
  // Draw activated checkboxes
  _tft->fillRect(TFT_WIDTH * 1 / 7 + 4, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 4, boxWidth - 8, boxWidth - 8, cleaningAll || _currentSetting == eLiquid1 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);
  _tft->fillRect(TFT_WIDTH * 3 / 7 + 4, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 4, boxWidth - 8, boxWidth - 8, cleaningAll || _currentSetting == eLiquid2 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);
  _tft->fillRect(TFT_WIDTH * 5 / 7 + 4, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 4, boxWidth - 8, boxWidth - 8, cleaningAll || _currentSetting == eLiquid3 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);

  // Draw liquid names
  _tft->setTextColor(TFT_COLOR_LIQUID_1);
  DrawCenteredString(liquid1String, TFT_WIDTH * 1 / 7 + boxWidth / 2, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 2 * boxWidth, false, 0);
  _tft->setTextColor(TFT_COLOR_LIQUID_2);
  DrawCenteredString(liquid2String, TFT_WIDTH * 3 / 7 + boxWidth / 2, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 2 * boxWidth, false, 0);
  _tft->setTextColor(TFT_COLOR_LIQUID_3);
  DrawCenteredString(liquid3String, TFT_WIDTH * 5 / 7 + boxWidth / 2, HEADEROFFSET_Y + TFT_HEIGHT / 3 + 2 * boxWidth, false, 0);
}

//===============================================================
// Draws the legend
//===============================================================
void DisplayDriver::DrawLegend()
{
  _tft->drawRect(X_LEGEND, Y_LEGEND, WIDTH_LEGEND, HEIGHT_LEGEND, TFT_COLOR_FOREGROUND);

  int16_t marginTop = 10;
  int16_t marginBetween = 21;
  int16_t boxWidth = 30;
  int16_t boxHeight = 10;
  int16_t boxMid = X_LEGEND + WIDTH_LEGEND / 2;

  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_BODY);
  
  _tft->fillRect(X_LEGEND + WIDTH_LEGEND / 2 - boxWidth / 2, Y_LEGEND + marginTop +                 0 * LOONGLINEOFFSET, boxWidth, boxHeight, TFT_COLOR_LIQUID_1);
  DrawCenteredString(liquid1String, boxMid,                  Y_LEGEND + marginTop + marginBetween + 0 * LOONGLINEOFFSET, true, _currentSetting == eLiquid1 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
  
  _tft->fillRect(X_LEGEND + WIDTH_LEGEND / 2 - boxWidth / 2, Y_LEGEND + marginTop +                 1 * LOONGLINEOFFSET, boxWidth, boxHeight, TFT_COLOR_LIQUID_2);
  DrawCenteredString(liquid2String, boxMid,                  Y_LEGEND + marginTop + marginBetween + 1 * LOONGLINEOFFSET, true, _currentSetting == eLiquid2 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
  
  _tft->fillRect(X_LEGEND + WIDTH_LEGEND / 2 - boxWidth / 2, Y_LEGEND + marginTop +                 2 * LOONGLINEOFFSET, boxWidth, boxHeight, TFT_COLOR_LIQUID_3);
  DrawCenteredString(liquid3String, boxMid,                  Y_LEGEND + marginTop + marginBetween + 2 * LOONGLINEOFFSET, true, _currentSetting == eLiquid3 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
}

//===============================================================
// Draws current values
//===============================================================
void DisplayDriver::DrawCurrentValues(bool isfullUpdate)
{
  String liquid1_PercentageString = FormatValue(_liquid1_Percentage, 2, 0) + String("%");
  String liquid2_PercentageString = FormatValue(_liquid2_Percentage, 2, 0) + String("%");
  String liquid3_PercentageString = FormatValue(_liquid3_Percentage, 2, 0) + String("%");

  int16_t lineX = 15;
  int16_t lineY = HEADEROFFSET_Y + 25;
  
  // Set text size
  _tft->setTextSize(1);
  
  if (isfullUpdate)
  {
    // Draw base string
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->setCursor(lineX, lineY);
    _tft->print("Mix [");
    _tft->setCursor(lineX + 80, lineY);
    _tft->print(",");
    _tft->setCursor(lineX + 130, lineY);
    _tft->print(",");
    _tft->setCursor(lineX + 185, lineY);
    _tft->print("]");
  }

  if (_lastDraw_Liquid1String != liquid1_PercentageString || isfullUpdate)
  {
    // Reset old string on display
    _tft->setTextColor(TFT_COLOR_BACKGROUND);
    _tft->setCursor(lineX + 40, lineY);
    _tft->print(_lastDraw_Liquid1String);
    
    // Draw new string on display
    _tft->setTextColor(TFT_COLOR_LIQUID_1);
    _tft->setCursor(lineX + 40, lineY);
    _tft->print(liquid1_PercentageString);
  }
    
  if (_lastDraw_Liquid2String != liquid2_PercentageString || isfullUpdate)
  {
    // Reset old string on display
    _tft->setTextColor(TFT_COLOR_BACKGROUND);
    _tft->setCursor(lineX + 90, lineY);
    _tft->print(_lastDraw_Liquid2String);
    
    // Draw new string on display
    _tft->setTextColor(TFT_COLOR_LIQUID_2);
    _tft->setCursor(lineX + 90, lineY);
    _tft->print(liquid2_PercentageString);
  }
  
  if (_lastDraw_Liquid3String != liquid3_PercentageString || isfullUpdate)
  {
    // Reset old string on display
    _tft->setTextColor(TFT_COLOR_BACKGROUND);
    _tft->setCursor(lineX + 140, lineY);
    _tft->print(_lastDraw_Liquid3String);
    
    // Draw new string on display
    _tft->setTextColor(TFT_COLOR_LIQUID_3);
    _tft->setCursor(lineX + 140, lineY);
    _tft->print(liquid3_PercentageString);
  }
  
  _lastDraw_Liquid1String = liquid1_PercentageString;
  _lastDraw_Liquid2String = liquid2_PercentageString;
  _lastDraw_Liquid3String = liquid3_PercentageString;
}

//===============================================================
// Draws full doughnut chart
//===============================================================
void DisplayDriver::DrawDoughnutChart3()
{
  DrawDoughnutChart3(false, true);
}

//===============================================================
// Draws doughnut chart
//===============================================================
void DisplayDriver::DrawDoughnutChart3(bool clockwise, bool isfullUpdate)
{ 
  if (isfullUpdate)
  {
    // Calculate count of draw_Angle's to draw
    int16_t liquid1Distance_Degrees = GetDistanceDegrees(_liquid1Angle_Degrees, _liquid2Angle_Degrees);
    int16_t liquid2Distance_Degrees = GetDistanceDegrees(_liquid2Angle_Degrees, _liquid3Angle_Degrees);
    int16_t liquid3Distance_Degrees = GetDistanceDegrees(_liquid3Angle_Degrees, _liquid1Angle_Degrees);
  
    // Draw doughnut chart parts
    FillArc(_liquid1Angle_Degrees, liquid1Distance_Degrees, TFT_COLOR_LIQUID_1);
    FillArc(_liquid2Angle_Degrees, liquid2Distance_Degrees, TFT_COLOR_LIQUID_2);
    FillArc(_liquid3Angle_Degrees, liquid3Distance_Degrees, TFT_COLOR_LIQUID_3);
  }
  else
  {
    DrawPartial(_liquid1Angle_Degrees, _lastDraw_liquid1Angle_Degrees, TFT_COLOR_LIQUID_1, TFT_COLOR_LIQUID_3, clockwise);
    DrawPartial(_liquid2Angle_Degrees, _lastDraw_liquid2Angle_Degrees, TFT_COLOR_LIQUID_2, TFT_COLOR_LIQUID_1, clockwise);
    DrawPartial(_liquid3Angle_Degrees, _lastDraw_liquid3Angle_Degrees, TFT_COLOR_LIQUID_3, TFT_COLOR_LIQUID_2, clockwise);
  }

  // Draw black spacer and selected white
  FillArc(Move360(_liquid1Angle_Degrees, -SPACERANGLE_DEGREES), 2 * SPACERANGLE_DEGREES, _currentSetting == eLiquid1 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
  FillArc(Move360(_liquid2Angle_Degrees, -SPACERANGLE_DEGREES), 2 * SPACERANGLE_DEGREES, _currentSetting == eLiquid2 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
  FillArc(Move360(_liquid3Angle_Degrees, -SPACERANGLE_DEGREES), 2 * SPACERANGLE_DEGREES, _currentSetting == eLiquid3 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
  
  // Set last drawn angles
  _lastDraw_liquid1Angle_Degrees = _liquid1Angle_Degrees;
  _lastDraw_liquid2Angle_Degrees = _liquid2Angle_Degrees;
  _lastDraw_liquid3Angle_Degrees = _liquid3Angle_Degrees;
}

//===============================================================
// Only draws partial update of arcs
//===============================================================
void DisplayDriver::DrawPartial(int16_t newAngle, int16_t lastAngle,  uint16_t colorAfter, uint16_t colorBefore, bool clockwise)
{
  if (lastAngle != newAngle)
  {
    // Calculate start angle and color
    // Draw from last angle to new angle
    int16_t startAngle = Move360(lastAngle, clockwise ? -SPACERANGLE_DEGREES : SPACERANGLE_DEGREES);
    uint16_t color = clockwise ? colorBefore : colorAfter;

    // Calculate draw distance
    int16_t distance_Degrees = clockwise ? GetDistanceDegrees(lastAngle, newAngle) : -360 + GetDistanceDegrees(lastAngle, newAngle);
    
    // Draw doughnut part
    FillArc(startAngle, distance_Degrees, color);
  }
}

//===============================================================
// Draw an arc with a defined thickness
//===============================================================
void DisplayDriver::FillArc(int16_t start_angle, int16_t distance_Degrees, uint16_t color)
{
  // x,y == coords of centre of arc
  // start_angle = 0 - 359
  // distance_Degrees = absolut distance to draw in degrees
  // r_inner = inner radius
  // r_outer = outer radius
  // color = 16 bit color value

  // Draw color blocks every 1°
  int16_t drawAngle_Degrees = distance_Degrees > 0 ? 1 : - 1;
  for (int16_t i = start_angle; i != start_angle + distance_Degrees; i += drawAngle_Degrees)
  {
    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * TFT_DEG2RAD);
    float sy = sin((i - 90) * TFT_DEG2RAD);
    int16_t x0 = sx * R_INNER_DOUGHNUTCHART + X0_DOUGHNUTCHART;
    int16_t y0 = sy * R_INNER_DOUGHNUTCHART + Y0_DOUGHNUTCHART;
    int16_t x1 = sx * R_OUTER_DOUGHNUTCHART + X0_DOUGHNUTCHART;
    int16_t y1 = sy * R_OUTER_DOUGHNUTCHART + Y0_DOUGHNUTCHART;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + drawAngle_Degrees - 90) * TFT_DEG2RAD);
    float sy2 = sin((i + drawAngle_Degrees - 90) * TFT_DEG2RAD);
    int16_t x2 = sx2 * R_INNER_DOUGHNUTCHART + X0_DOUGHNUTCHART;
    int16_t y2 = sy2 * R_INNER_DOUGHNUTCHART + Y0_DOUGHNUTCHART;
    int16_t x3 = sx2 * R_OUTER_DOUGHNUTCHART + X0_DOUGHNUTCHART;
    int16_t y3 = sy2 * R_OUTER_DOUGHNUTCHART + Y0_DOUGHNUTCHART;

    _tft->fillTriangle(x0, y0, x1, y1, x2, y2, color);
    _tft->fillTriangle(x1, y1, x2, y2, x3, y3, color);
  }
}

//===============================================================
// Draws bargraph
//===============================================================
void DisplayDriver::DrawBargraph(bool isfullUpdate)
{
  int16_t bargraphY = HEADEROFFSET_Y + 45;
  int16_t lineY = HEADEROFFSET_Y + 185;
  uint16_t bargraphMax = 125;

  if (isfullUpdate)
  {
    // Draw empty bar graph hulls
    _tft->drawRect(1 * BARGRAPH_MARGIN_HORI - 1, bargraphY - 1, BARGRAPH_MARGIN_HORI + 2, bargraphMax + 2, TFT_COLOR_FOREGROUND);
    _tft->drawRect(3 * BARGRAPH_MARGIN_HORI - 1, bargraphY - 1, BARGRAPH_MARGIN_HORI + 2, bargraphMax + 2, TFT_COLOR_FOREGROUND);
    _tft->drawRect(5 * BARGRAPH_MARGIN_HORI - 1, bargraphY - 1, BARGRAPH_MARGIN_HORI + 2, bargraphMax + 2, TFT_COLOR_FOREGROUND);
  }
  
  if (_lastDrawCurrentSetting != _currentSetting || isfullUpdate)
  {
    // Draw bargraph names
    _tft->setTextColor(TFT_COLOR_LIQUID_1);
    DrawCenteredString(liquid1String, 1 * BARGRAPH_MARGIN_HORI + BARGRAPH_MARGIN_HORI / 2, lineY, true, _currentSetting == eLiquid1 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
    _tft->setTextColor(TFT_COLOR_LIQUID_2);
    DrawCenteredString(liquid2String, 3 * BARGRAPH_MARGIN_HORI + BARGRAPH_MARGIN_HORI / 2, lineY, true, _currentSetting == eLiquid2 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);
    _tft->setTextColor(TFT_COLOR_LIQUID_3);
    DrawCenteredString(liquid3String, 5 * BARGRAPH_MARGIN_HORI + BARGRAPH_MARGIN_HORI / 2, lineY, true, _currentSetting == eLiquid3 ? TFT_COLOR_FOREGROUND : TFT_COLOR_BACKGROUND);

    _lastDrawCurrentSetting = _currentSetting;
  }

  if (_lastDraw_liquid1_Percentage != _liquid1_Percentage || isfullUpdate)
  {
    uint16_t liquid1Height =     bargraphMax * _liquid1_Percentage          / 100.0;
    uint16_t lastLiquid1Height = bargraphMax * _lastDraw_liquid1_Percentage / 100.0;

    // Reset old bargraph and draw new one on display
    _tft->fillRect(1 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - lastLiquid1Height, BARGRAPH_MARGIN_HORI, lastLiquid1Height, TFT_COLOR_BACKGROUND);
    _tft->fillRect(1 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - liquid1Height,     BARGRAPH_MARGIN_HORI, liquid1Height,     TFT_COLOR_LIQUID_1);
  }
    
  if (_lastDraw_liquid2_Percentage != _liquid2_Percentage || isfullUpdate)
  {
    uint16_t liquid2Height =     bargraphMax * _liquid2_Percentage          / 100.0;
    uint16_t lastLiquid2Height = bargraphMax * _lastDraw_liquid2_Percentage / 100.0;

    // Reset old bargraph and draw new one on display
    _tft->fillRect(3 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - lastLiquid2Height, BARGRAPH_MARGIN_HORI, lastLiquid2Height, TFT_COLOR_BACKGROUND);
    _tft->fillRect(3 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - liquid2Height,     BARGRAPH_MARGIN_HORI, liquid2Height,     TFT_COLOR_LIQUID_2);
  }
  
  if (_lastDraw_liquid3_Percentage != _liquid3_Percentage || isfullUpdate)
  {
    uint16_t liquid3Height =     bargraphMax * _liquid3_Percentage          / 100.0;
    uint16_t lastLiquid3Height = bargraphMax * _lastDraw_liquid3_Percentage / 100.0;

    // Reset old bargraph and draw new one on display
    _tft->fillRect(5 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - lastLiquid3Height, BARGRAPH_MARGIN_HORI, lastLiquid3Height, TFT_COLOR_BACKGROUND);
    _tft->fillRect(5 * BARGRAPH_MARGIN_HORI, bargraphY + bargraphMax - liquid3Height,     BARGRAPH_MARGIN_HORI, liquid3Height,     TFT_COLOR_LIQUID_3);
  }
  
  _lastDraw_liquid1_Percentage = _liquid1_Percentage;
  _lastDraw_liquid2_Percentage = _liquid2_Percentage;
  _lastDraw_liquid3_Percentage = _liquid3_Percentage;
}

//===============================================================
// Draws a string centered
//===============================================================
void DisplayDriver::DrawCenteredString(const String &text, int16_t x, int16_t y, bool underlined, uint16_t lineColor)
{
  // Get text bounds
  int16_t x1, y1;
  uint16_t w, h;
  _tft->getTextBounds(text, x, y, &x1, &y1, &w, &h);

  // Calculate cursor position
  int16_t x_text = x - w / 2;
  int16_t y_text = y + h / 2;
  _tft->setCursor(x_text, y_text);
  
  // Print text
  _tft->print(text);

  // Underline if active
  if (underlined)
  {
    _tft->drawLine(x_text, y + h, x_text + w, y + h, lineColor);
  }
}

//===============================================================
// Formats double value
//===============================================================
String DisplayDriver::FormatValue(double value, int mainPlaces, int decimalPlaces)
{
  String returnValue = "";

  dtostrf(value, mainPlaces, decimalPlaces, output);
  returnValue += String(output);

  return returnValue;
}
