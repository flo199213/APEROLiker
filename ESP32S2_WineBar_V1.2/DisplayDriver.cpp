/**
 * Includes all display functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

#include "DisplayDriver.h"

//===============================================================
// Global variables
//===============================================================
 DisplayDriver Display;

//===============================================================
// Constructor
//===============================================================
DisplayDriver::DisplayDriver()
{
}

//===============================================================
// Initializes the display driver
//===============================================================
void DisplayDriver::Begin(Adafruit_ST7789* tft, bool spiffsAvailable)
{
  // Set display variable
  _tft = tft;

  // Initialize display
  _tft->init(TFT_WIDTH, TFT_HEIGHT, SPI_MODE3);
  _tft->invertDisplay(true);
  _tft->setRotation(3);
  _tft->setTextWrap(false);
  _tft->setFont(&FreeSans9pt7b);
  _tft->fillScreen(TFT_COLOR_BACKGROUND);

  int16_t x = TFT_WIDTH / 2;
  int16_t y = TFT_HEIGHT / 2;

  // Show starting message
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Booting...", x, y);

  // Create image objects
  _imageBottle = new SPIFFSImage();
  _imageGlass = new SPIFFSImage();
  _imageLogo = new SPIFFSImage();
  _imageBottleWhiteWine = new SPIFFSImage();
  _imageBottleRoseWine = new SPIFFSImage();
  _imageBottleSparklingWater = new SPIFFSImage();

  // Load startup images to RAM
  if (spiffsAvailable)
  {
    // Set images available
    _imagesAvailable = (reader.LoadBMP(startupImageBottle.c_str(), _imageBottle) == IMAGE_SUCCESS &&
      reader.LoadBMP(startupImageGlass.c_str(), _imageGlass) == IMAGE_SUCCESS &&
      reader.LoadBMP(startupImageLogo.c_str(), _imageLogo) == IMAGE_SUCCESS &&
      reader.LoadBMP(imageBottleWhiteWine.c_str(), _imageBottleWhiteWine) == IMAGE_SUCCESS &&
      reader.LoadBMP(imageBottleRoseWine.c_str(), _imageBottleRoseWine) == IMAGE_SUCCESS &&
      reader.LoadBMP(imageBottleSparklingWater.c_str(), _imageBottleSparklingWater) == IMAGE_SUCCESS) ? IMAGE_SUCCESS : IMAGE_ERR_FILE_NOT_FOUND;
  }
  else
  {
    // Debug information on display
    DrawCenteredString("SPIFFS Failed", x, y + SHORTLINEOFFSET);
    delay(3000);
  }
}

//===============================================================
// Sets the menu state
//===============================================================
void DisplayDriver::SetMenuState(MixerState state)
{
  _menuState = state;
}

//===============================================================
// Sets the current liquid value
//===============================================================
void DisplayDriver::SetDashboardLiquid(MixtureLiquid liquid)
{
  _dashboardLiquid = liquid;
}

//===============================================================
// Sets the cleaning liquid value
//===============================================================
void DisplayDriver::SetCleaningLiquid(MixtureLiquid liquid)
{
  _cleaningLiquid = liquid;
}

//===============================================================
// Sets the percentage values
//===============================================================
void DisplayDriver::SetPercentages(int16_t liquid1_Percentage, int16_t liquid2_Percentage, int16_t liquid3_Percentage)
{
  _liquid1_Percentage = liquid1_Percentage;
  _liquid2_Percentage = liquid2_Percentage;
  _liquid3_Percentage = liquid3_Percentage;
}

//===============================================================
// Sets the bar stock
//===============================================================
void DisplayDriver::SetBar(BarBottle barBottle1, BarBottle barBottle2, BarBottle barBottle3)
{
  _barBottle1 = barBottle1;
  _barBottle2 = barBottle2;
  _barBottle3 = barBottle3;
}

//===============================================================
// Shows intro page
//===============================================================
void DisplayDriver::ShowIntroPage()
{
  // Draw intro page background
  _tft->fillRect(0, 0,                TFT_WIDTH, TFT_HEIGHT * 0.8, TFT_COLOR_STARTPAGE_BACKGROUND);
  _tft->fillRect(0, TFT_HEIGHT * 0.8, TFT_WIDTH, TFT_HEIGHT * 0.2, TFT_COLOR_STARTPAGE_FOREGROUND);

  if (_imagesAvailable == IMAGE_SUCCESS)
  {
    // Draw intro images
    _imageBottle->Draw(TFT_BOTTLE_POS_X, TFT_BOTTLE_POS_Y, _tft, TFT_TRANSPARENCY_COLOR);
    _imageGlass->Draw(TFT_GLASS_POS_X,   TFT_GLASS_POS_Y,  _tft, TFT_TRANSPARENCY_COLOR);
    _imageLogo->Draw(TFT_LOGO_POS_X,     TFT_LOGO_POS_Y,   _tft, TFT_TRANSPARENCY_COLOR);

    // Free memory
    delete _imageGlass;
    //delete _imageBottle; // Do NOT delete bottle image for usage with dashboard!
    //delete _imageLogo;   // Do NOT delete logo image for usage with screen saver!
  }
  else
  {
    // Draw info box (fallback)
    DrawInfoBox("- Startpage -", "NO SPIFFS Files!");
  }
}

//===============================================================
// Shows help page
//===============================================================
void DisplayDriver::ShowHelpPage()
{
  int16_t x = 15;
  int16_t y = HEADEROFFSET_Y + 20;

  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Instructions", false);

  // Set text settings
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_BODY);

  // Draw help text
  _tft->setCursor(x, y);
  _tft->print("Short Press:");
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print(" -> Change Setting");
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(LIQUID1_NAME);
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(LIQUID2_NAME);
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print("    ~ ");
  _tft->print(LIQUID3_NAME);
  
  _tft->setCursor(x, y += LONGLINEOFFSET);
  _tft->print("Rotate:");
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print(" -> Change Spritzer");

  _tft->setCursor(x, y += LONGLINEOFFSET);
  _tft->print("Long Press:");
  _tft->setCursor(x, y += SHORTLINEOFFSET);  
  _tft->print(" -> Menu/Go Back");
}

//===============================================================
// Shows menu page
//===============================================================
void DisplayDriver::ShowMenuPage()
{
    // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Menu");

  // Draw menu
  DrawMenu(true);
}

//===============================================================
// Shows dashboard page
//===============================================================
void DisplayDriver::ShowDashboardPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader();
  
  // Draw bar
  Display.DrawBar(true, true);
  
  int16_t x = TFT_WIDTH / 2;
  int16_t y = 65;

  // Print selection text
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Select WINE for dispensing:", x, y, false, true, 0, 0x528A); // Gray
}

//===============================================================
// Shows cleaning page
//===============================================================
void DisplayDriver::ShowCleaningPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Cleaning Mode");

  int16_t x = TFT_WIDTH / 2;
  int16_t y = TFT_HEIGHT / 3;
  
  // Print selection text
  _tft->setTextColor(TFT_COLOR_FOREGROUND);
  DrawCenteredString("Select pumps for cleaning:", x, y);

  // Draw checkboxes
  DrawCheckBoxes(_cleaningLiquid);
}
//===============================================================
// Shows bar page
//===============================================================
void DisplayDriver::ShowBarPage()
{
// Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Bar Stock");
  
  // Draw bar
  Display.DrawBar(false, true);
}

//===============================================================
// Shows settings page
//===============================================================
void DisplayDriver::ShowSettingsPage()
{
  int16_t x = 15;
  int16_t y = HEADEROFFSET_Y + 25;

  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);
  
  // Draw header information
  DrawHeader("Settings");

  double valueLiquid1 = FlowMeter.GetValueLiquid1();
  double valueLiquid2 = FlowMeter.GetValueLiquid2();
  double valueLiquid3 = FlowMeter.GetValueLiquid3();

  // Fill in settings text
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_BODY);

  _tft->setCursor(x, y);
  _tft->print("App Version: ");
  _tft->print(APP_VERSION);

  DrawSettings(true);

  _tft->setCursor(x, y += (SHORTLINEOFFSET + 2 * LONGLINEOFFSET));
  _tft->print("Volume of liquid filled:");
  
  // Draw liquid 1 flow meter value
  _tft->setTextColor(TFT_COLOR_LIQUID_1);
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print(LIQUID1_NAME);
  _tft->print(":");
  _tft->setCursor(x + 120, y);
  _tft->print(FormatValue(valueLiquid1, 4, 2));
  _tft->print(" L");
  
  // Draw liquid 2 flow meter value
  _tft->setTextColor(TFT_COLOR_LIQUID_2);
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print(LIQUID2_NAME);
  _tft->print(":");
  _tft->setCursor(x + 120, y);
  _tft->print(FormatValue(valueLiquid2, 4, 2));
  _tft->print(" L");  
  
  // Draw liquid 3 flow meter value
  _tft->setTextColor(TFT_COLOR_LIQUID_3);
  _tft->setCursor(x, y += SHORTLINEOFFSET);
  _tft->print(LIQUID3_NAME);
  _tft->print(":");
  _tft->setCursor(x + 120, y);
  _tft->print(FormatValue(valueLiquid3, 4, 2));
  _tft->print(" L");
  
  x = 40;
  y = TFT_HEIGHT - 20;

  // Draw copyright icon
  _tft->drawXBitmap(x, y, icon_copyright, 20, 20, TFT_COLOR_TEXT_BODY);
  
  // Draw copyright text
  _tft->setCursor(x + 25, y + 15);
  _tft->setTextColor(TFT_COLOR_TEXT_BODY);
  _tft->print("2024 F.Stablein");
  _tft->drawRect(x + 105, y + 2, 2, 2, TFT_COLOR_TEXT_BODY);  // Stablein with two dots -> Stäblein
  _tft->drawRect(x + 109, y + 2, 2, 2, TFT_COLOR_TEXT_BODY);  // Stablein with two dots -> Stäblein
}

//===============================================================
// Shows screen saver page
//===============================================================
void DisplayDriver::ShowScreenSaverPage()
{
  // Clear screen
  _tft->fillScreen(TFT_COLOR_BACKGROUND);

  // Draw inital screen saver
  DrawScreenSaver();
}

//===============================================================
// Draws default header Text
//===============================================================
void DisplayDriver::DrawHeader()
{
  DrawHeader(String("-- ") + MIXER_NAME + " --");
}

//===============================================================
// Draws header Text
//===============================================================
void DisplayDriver::DrawHeader(const String &text, bool withIcons)
{
  int16_t x = TFT_WIDTH / 2;
  int16_t y = HEADEROFFSET_Y / 2;

  // Draw header text
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_TEXT_HEADER);
  DrawCenteredString(text, x, y);

  x = HEADER_MARGIN;
  y = HEADEROFFSET_Y;
  int16_t x1 = TFT_WIDTH - (2 * HEADER_MARGIN);
  int16_t y1 = HEADEROFFSET_Y;

  // Draw header line
  _tft->drawLine(x, y, x1, y1, TFT_COLOR_FOREGROUND);

  if (withIcons)
  {
#if defined(WIFI_MIXER)
    // Draw wifi icons
    DrawWifiIcons(true);
#endif
  }
}

#if defined(WIFI_MIXER)
//===============================================================
// Draws the Wifi icon
//===============================================================
void DisplayDriver::DrawWifiIcons(bool isfullUpdate)
{
  int16_t x = TFT_WIDTH - 24 - 5;
  int16_t y = 2;
  int16_t width = 24;
  int16_t height = 24;

  wifi_mode_t wifiMode = Wifihandler.GetWifiMode();
  uint16_t connectedClients = Wifihandler.GetConnectedClients();
  
  // Check connected clients for changed value
  if (_lastDraw_ConnectedClients == connectedClients && !isfullUpdate)
  {
    return;
  }
  _lastDraw_ConnectedClients = connectedClients;

  // Clear wifi icon
  _tft->drawXBitmap(x, y, icon_wifi, width, height, TFT_COLOR_BACKGROUND);
  _tft->drawXBitmap(x, y, icon_noWifi, width, height, TFT_COLOR_BACKGROUND);

  // Draw new wifi icon
  _tft->drawXBitmap(x, y, wifiMode == WIFI_MODE_AP ? icon_wifi : icon_noWifi, width, height, TFT_COLOR_FOREGROUND);

  x = 5;
  y += 2;

  // Clear connected clients
  _tft->fillRect(x, y, width, height, TFT_COLOR_BACKGROUND);

  if (wifiMode == WIFI_MODE_AP)
  {
    // Draw new connected clients
    _tft->drawXBitmap(x, y, icon_device, width, height, TFT_COLOR_FOREGROUND);
    _tft->setCursor(x + 7, y + 17);
    _tft->setTextColor(TFT_COLOR_FOREGROUND);
    _tft->print(connectedClients);
  }
}
#endif

//===============================================================
// Draws the info box
//===============================================================
void DisplayDriver::DrawInfoBox(const String &line1, const String &line2)
{
  int16_t x = INFOBOX_MARGIN_HORI;
  int16_t y = HEADEROFFSET_Y + INFOBOX_MARGIN_VERT;
  int16_t width = TFT_WIDTH - 2 * INFOBOX_MARGIN_HORI;
  int16_t height = TFT_HEIGHT - HEADEROFFSET_Y - 2 * INFOBOX_MARGIN_VERT;

  // Draw rectangle with colored border
  _tft->fillRoundRect(x,     y,      width,     height,     INFOBOX_CORNERRADIUS, TFT_COLOR_INFOBOX_BORDER);
  _tft->fillRoundRect(x + 2, y + 2,  width - 4, height - 4, INFOBOX_CORNERRADIUS, TFT_COLOR_INFOBOX_BACKGROUND);
  
  // Move to the middle of the box
  x += width / 2;
  y += height / 2;

  // Fill in info text
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_COLOR_INFOBOX_FOREGROUND);
  DrawCenteredString(line1, x, y - (SHORTLINEOFFSET / 2));
  DrawCenteredString(line2, x, y + (SHORTLINEOFFSET / 2));
}

//===============================================================
// Draws the menu
//===============================================================
void DisplayDriver::DrawMenu(bool isfullUpdate)
{
  int16_t x = 0;
  int16_t y = 0;
  int16_t width = 0;
  int16_t height = 0;

  int16_t marginToHeader = 40;

  if (isfullUpdate)
  {
    x = MENU_MARGIN_HORI + MENU_MARGIN_ICON;
    y = HEADEROFFSET_Y + marginToHeader - 22;
    width = 32;
    height = 32;

    // Draw icons
    _tft->drawXBitmap(x, y,                    icon_dashboard, width, height, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(x, y += MENU_LINEOFFSET, icon_cleaning,  width, height, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(x, y += MENU_LINEOFFSET, icon_cocktails, width, height, TFT_COLOR_FOREGROUND);
    _tft->drawXBitmap(x, y += MENU_LINEOFFSET, icon_settings,  width, height, TFT_COLOR_FOREGROUND);

    x = MENU_MARGIN_HORI + MENU_MARGIN_ICON + MENU_MARGIN_TEXT;
    y = HEADEROFFSET_Y + marginToHeader;

    // Draw menu text
    _tft->setTextSize(1);
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->setCursor(x, y);
    _tft->print("Dashboard");
    _tft->setCursor(x, y += MENU_LINEOFFSET);
    _tft->print("Cleaning Mode");
    _tft->setCursor(x, y += MENU_LINEOFFSET);
    _tft->print("Bar Stock");
    _tft->setCursor(x, y += MENU_LINEOFFSET);
    _tft->print("Settings");
  }

  if (_lastDraw_MenuState != _menuState || isfullUpdate)
  {
    x = MENU_MARGIN_HORI - 2;
    y = HEADEROFFSET_Y + marginToHeader + ((uint16_t)_lastDraw_MenuState - 1) * MENU_LINEOFFSET - 6 - MENU_SELECTOR_HEIGHT / 2;
    width = TFT_WIDTH - 2 * MENU_MARGIN_HORI;
    height = MENU_SELECTOR_HEIGHT;

    // Reset old menu selection on display
    _tft->drawRoundRect(x, y, width, height, MENU_SELECTOR_CORNERRADIUS, TFT_COLOR_BACKGROUND);

    y = HEADEROFFSET_Y + marginToHeader + ((uint16_t)_menuState - 1) * MENU_LINEOFFSET - 6 - MENU_SELECTOR_HEIGHT / 2;

    // Draw new menu selection on display
    _tft->drawRoundRect(x, y, width, height, MENU_SELECTOR_CORNERRADIUS, TFT_COLOR_MENU_SELECTOR);

    // Save last state
    _lastDraw_MenuState = _menuState;
  }
}

//===============================================================
// Draw bar
//===============================================================
void DisplayDriver::DrawBar(bool isDashboard, bool isfullUpdate)
{
  // Draw only check boxes if complete bar stock is empty
  if (isDashboard &&
    _barBottle1 == eEmpty &&
    _barBottle2 == eEmpty &&
    _barBottle3 == eEmpty)
  {
    // Draw checkboxes
    DrawCheckBoxes(_dashboardLiquid);
    return;
  }

  int16_t spacing = 78;
  int16_t x0 = TFT_WIDTH / 2; // Mid screen
  int16_t y = HEADEROFFSET_Y + 10;

  DrawBarPart(x0 - spacing, y, eLiquid1, _barBottle1, _lastDraw_barBottle1, _liquid1_Percentage, _lastDraw_liquid1_Percentage, LIQUID1_NAME, TFT_COLOR_LIQUID_1, isDashboard, isfullUpdate);
  DrawBarPart(x0,           y, eLiquid2, _barBottle2, _lastDraw_barBottle2, _liquid2_Percentage, _lastDraw_liquid2_Percentage, LIQUID2_NAME, TFT_COLOR_LIQUID_2, isDashboard, isfullUpdate);
  DrawBarPart(x0 + spacing, y, eLiquid3, _barBottle3, _lastDraw_barBottle3, _liquid3_Percentage, _lastDraw_liquid3_Percentage, LIQUID3_NAME, TFT_COLOR_LIQUID_3, isDashboard, isfullUpdate);

  _lastDraw_SelectedLiquid = _dashboardLiquid;
  _lastDraw_barBottle1 = _barBottle1;
  _lastDraw_barBottle2 = _barBottle2;
  _lastDraw_barBottle3 = _barBottle3;
  _lastDraw_liquid1_Percentage = _liquid1_Percentage;
  _lastDraw_liquid2_Percentage = _liquid2_Percentage;
  _lastDraw_liquid3_Percentage = _liquid3_Percentage;
}

//===============================================================
// Draw checkboxes
//===============================================================
void DisplayDriver::DrawCheckBoxes(MixtureLiquid liquid)
{
  int16_t boxSize = 30;
  int16_t spacing = 78;
  int16_t x0 = TFT_WIDTH / 2; // Mid screen
  int16_t y = HEADEROFFSET_Y + 80;
  
  // Draw checkboxes
  _tft->drawRect(x0 - boxSize / 2 - spacing, y, boxSize, boxSize, TFT_COLOR_FOREGROUND);
  _tft->drawRect(x0 - boxSize / 2,           y, boxSize, boxSize, TFT_COLOR_FOREGROUND);
  _tft->drawRect(x0 - boxSize / 2 + spacing, y, boxSize, boxSize, TFT_COLOR_FOREGROUND);
  
  // Draw activated checkboxes
  _tft->fillRect(x0 - boxSize / 2 + 4 - spacing, y + 4, boxSize - 8, boxSize - 8, liquid == eLiquidAll || liquid == eLiquid1 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);
  _tft->fillRect(x0 - boxSize / 2 + 4,           y + 4, boxSize - 8, boxSize - 8, liquid == eLiquidAll || liquid == eLiquid2 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);
  _tft->fillRect(x0 - boxSize / 2 + 4 + spacing, y + 4, boxSize - 8, boxSize - 8, liquid == eLiquidAll || liquid == eLiquid3 ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);

  // Move under the boxes for liquid names
  y = HEADEROFFSET_Y + 140;

  // Draw liquid names
  _tft->setTextColor(TFT_COLOR_LIQUID_1);
  DrawCenteredString(LIQUID1_NAME, x0 - spacing, y);
  _tft->setTextColor(TFT_COLOR_LIQUID_2);
  DrawCenteredString(LIQUID2_NAME, x0, y);
  _tft->setTextColor(TFT_COLOR_LIQUID_3);
  DrawCenteredString(LIQUID3_NAME, x0 + spacing, y);
}

//===============================================================
// Draws settings
//===============================================================
void DisplayDriver::DrawSettings(bool isfullUpdate)
{
  int16_t x = 15;
  int16_t y = HEADEROFFSET_Y + 25 + LONGLINEOFFSET;

  if (isfullUpdate)
  {
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->setCursor(x, y);
    _tft->print("PWM CycleTime: ");
  }

  uint32_t cycleTimespan_ms = Pumps.GetCycleTimespan();

  if (_lastDraw_cycleTimespan_ms != cycleTimespan_ms || isfullUpdate)
  {
    // Clear old value
    _tft->setCursor(x + 145, y);
    _tft->setTextColor(TFT_COLOR_BACKGROUND);
    _tft->print(_lastDraw_cycleTimespan_ms);
    _tft->print(" ms");

    // Set new value
    _tft->setCursor(x + 145, y);
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->print(cycleTimespan_ms);
    _tft->print(" ms");

    _lastDraw_cycleTimespan_ms = cycleTimespan_ms;
  }

#if defined(WIFI_MIXER)
  wifi_mode_t wifiMode = Wifihandler.GetWifiMode();

  // Move to next line
  y += SHORTLINEOFFSET;

  if (isfullUpdate)
  {
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->setCursor(x, y);
    _tft->print("WIFI Mode: ");
  }

  if (_lastDraw_wifiMode != wifiMode || isfullUpdate)
  {
    // Clear old value
    _tft->setCursor(x + 98, y);
    _tft->setTextColor(TFT_COLOR_BACKGROUND);
    _tft->print(_lastDraw_wifiMode == WIFI_MODE_AP ? "AP" : "OFF");

    // Set new value
    _tft->setCursor(x + 98, y);
    _tft->setTextColor(TFT_COLOR_TEXT_BODY);
    _tft->print(wifiMode == WIFI_MODE_AP ? "AP" : "OFF");
    
    _lastDraw_wifiMode = wifiMode;
  }
#endif
}

//===============================================================
// Draws screen saver
//===============================================================
void DisplayDriver::DrawScreenSaver()
{
  bool hasLogo = _imagesAvailable == IMAGE_SUCCESS;
  int16_t logoWidth = hasLogo ? _imageLogo->Width() : 0;
  int16_t logoHeight = hasLogo ? _imageLogo->Height() : 0;
  
  // Move logo indexes
  int16_t logo_x = _lastLogo_x + _xDir;
  int16_t logo_y = _lastLogo_y + _yDir;
  
  // Move logo if image is available
  if (hasLogo)
  {
    _imageLogo->Move(_lastLogo_x, _lastLogo_y, logo_x, logo_y, _tft, TFT_COLOR_BACKGROUND, TFT_TRANSPARENCY_COLOR);
  }

  // Impact collision with the left or right edge
  if (logo_x <= -logoWidth / 2 || logo_x >= TFT_WIDTH - logoWidth / 2)
  {
    _xDir = -_xDir;
  }

  // Impact collision with the top or bottom edge
  if (logo_y <= -logoHeight / 2 || logo_y >= TFT_HEIGHT - logoHeight / 2)
  {
    _yDir = -_yDir;
  }

  // Draw stars
  for (int index = 0; index < SCREENSAVER_STARCOUNT; index++)
  {
    // Init new star, if star animation finished
    if (_stars[index].Size >= _stars[index].MaxSize)
    {
      // Clear old star only outside of the logo
      if (!hasLogo ||
        !(_stars[index].X > logo_x && _stars[index].X < logo_x + logoWidth &&
        _stars[index].Y > logo_y && _stars[index].Y < logo_y + logoHeight &&
        _imageLogo->GetPixel(_stars[index].X - logo_x, _stars[index].Y - logo_y) != TFT_TRANSPARENCY_COLOR))
      {
        DrawStar(_stars[index].X, _stars[index].Y, _stars[index].FullStars, TFT_COLOR_BACKGROUND, _stars[index].Size);
      }

      _stars[index].X = random(0, TFT_WIDTH);
      _stars[index].Y = random(0, TFT_HEIGHT);
      _stars[index].MaxSize = random(1, 6);
      _stars[index].FullStars = random(0, 12) < 6 ? true : false;
      _stars[index].Size = 0;
    }

    // Draw new star only outside of the logo
    if (!hasLogo || 
      !(_stars[index].X > logo_x && _stars[index].X < logo_x + logoWidth &&
      _stars[index].Y > logo_y && _stars[index].Y < logo_y + logoHeight &&
      _imageLogo->GetPixel(_stars[index].X - logo_x, _stars[index].Y - logo_y) != TFT_TRANSPARENCY_COLOR))
    {
      DrawStar(_stars[index].X, _stars[index].Y, _stars[index].FullStars, TFT_COLOR_FOREGROUND, _stars[index].Size);
    }

    // Increment star size
    _stars[index].Size++;
  }

  _lastLogo_x = logo_x;
  _lastLogo_y = logo_y;
}

//===============================================================
// Draws a star
//===============================================================
void DisplayDriver::DrawStar(int16_t x0, int16_t y0, bool fullStars, uint16_t color, int16_t size)
{
  _tft->writePixel(x0, y0, color);

  if (size > 0)
  {
    DrawStarTail(x0, y0, 1, 2, fullStars, color);
  }
  if (size > 1)
  {
    DrawStarTail(x0, y0, 4, 5, fullStars, color);
  }
  if (size > 2)
  {
    DrawStarTail(x0, y0, 7, 8, fullStars, color);
  }
  if (size > 3)
  {
    DrawStarTail(x0, y0, 10, 11, fullStars, color);
  }
  if (size > 4)
  {
    DrawStarTail(x0, y0, 13, 14, fullStars, color);
  }
}

//===============================================================
// Draws a star tail
//===============================================================
void DisplayDriver::DrawStarTail(int16_t x0, int16_t y0, int16_t start, int16_t end, bool fullStars, uint16_t color)
{
  // Nach oben
  _tft->writeLine(x0, y0 - start, x0, y0 - end, color);

  // Nach unten
  _tft->writeLine(x0, y0 + start, x0, y0 + end, color);

  // Nach rechts
  _tft->writeLine(x0 + start, y0, x0 + end, y0, color);

  // Nach links
  _tft->writeLine(x0 - start, y0, x0 - end, y0, color);

  if (fullStars)
  {
    // Nach rechts oben
    _tft->writeLine(x0 + start, y0 - start, x0 + end, y0 - end, color);

    // Nach links oben
    _tft->writeLine(x0 - start, y0 - start, x0 - end, y0 - end, color);

    // Nach rechts unten
    _tft->writeLine(x0 + start, y0 + start, x0 + end, y0 + end, color);

    // Nach links unten
    _tft->writeLine(x0 - start, y0 + start, x0 - end, y0 + end, color);
  }
}

//===============================================================
// Draws a part of the bar
//===============================================================
void DisplayDriver::DrawBarPart(int16_t x0, int16_t y, MixtureLiquid liquid, BarBottle barBottle, BarBottle lastDraw_barBottle, int16_t liquid_Percentage, int16_t lastDraw_liquid_Percentage, String name, uint16_t color, bool isDashboard, bool isfullUpdate)
{
  int16_t boxSize = 30;
  int16_t checkboxOffsetY = 70;
  int16_t namesOffsetX = 15;
  int16_t namesOffsetY = 175;
  
  bool isEmpty = barBottle == eEmpty;
  bool selectedChanged = _dashboardLiquid != _lastDraw_SelectedLiquid;
  bool bottleChanged = barBottle != lastDraw_barBottle;
  bool sparklingWaterChanged = liquid_Percentage != lastDraw_liquid_Percentage;
  bool isSelected = _dashboardLiquid == liquid;
  bool hasSparklingWater = _barBottle1 == eSparklingWater || _barBottle2 == eSparklingWater || _barBottle3 == eSparklingWater;

  // Reset old bottle type selection -> only if bottle type changed
  if (!isDashboard && bottleChanged)
  {
    SelectBarBottle(lastDraw_barBottle, x0, y, TFT_COLOR_BACKGROUND);
  }

  // Reset current bottle type selection -> only if selection changed
  if (!isDashboard && selectedChanged)
  {
    SelectBarBottle(barBottle, x0, y, TFT_COLOR_BACKGROUND);
  }

  // Clear current bottle type -> only if bottle type changed
  if (!isDashboard && bottleChanged)
  {
    ClearBarBottle(lastDraw_barBottle, barBottle, x0, y, TFT_COLOR_BACKGROUND);
  }
  
  // Draw current bottle selection -> only if selected AND (full update OR bottle changed OR selected changed)
  if (!isDashboard &&
    isSelected &&
    (isfullUpdate || bottleChanged || selectedChanged))
  {
    SelectBarBottle(barBottle, x0, y, TFT_COLOR_FOREGROUND);
  }
  
  // Draw current bottle and checkbox -> only if full update or bottle type changed
  if (isfullUpdate || bottleChanged)
  {
    // Draw bottle image
    DrawBarBottle(barBottle, x0, y);
  
    if (isDashboard && !isEmpty)
    {
      // Draw checkbox
      bool backgroundColor = barBottle == eWhiteWine || barBottle == eSparklingWater;
      _tft->drawRect(x0 - boxSize / 2, y + checkboxOffsetY, boxSize, boxSize, backgroundColor ? TFT_COLOR_BACKGROUND : TFT_COLOR_FOREGROUND);
    }
  }

  // Draw liquid name -> only if full update, bottle type changed or selected changed
  if (isfullUpdate || bottleChanged || selectedChanged)
  {
    // Draw liquid name
    _tft->setTextColor(color);
    _tft->fillRect(x0 - namesOffsetX - 17, y + namesOffsetY - 15, 54, 30, TFT_COLOR_BACKGROUND);
    DrawCenteredString(name, x0 - namesOffsetX, y + namesOffsetY);
  }

  // Draw checkbox infill -> only if full update, bottle type changed or selected changed
  if (isDashboard &&
    !isEmpty &&
    (isfullUpdate || bottleChanged || selectedChanged))
  {
    // Draw checkbox infill
    _tft->fillRect(x0 - boxSize / 2 + 4, y + checkboxOffsetY + 4, boxSize - 8, boxSize - 8, isSelected ? TFT_COLOR_STARTPAGE : TFT_COLOR_BACKGROUND);
  }

  // Draw sparkling water percentage -> only if any sparkling water is present and not beside itself
  if (isDashboard &&
    !isEmpty &&
    hasSparklingWater &&
    barBottle != eSparklingWater &&
    (isfullUpdate || sparklingWaterChanged))
  {
    int16_t x = x0 - 37;
    int16_t yTop = y + namesOffsetY - 120;

    // Draw bar graph
    _tft->fillRect(x, yTop, 3, 100 - liquid_Percentage, color);
    _tft->fillRect(x, yTop + 100 - liquid_Percentage, 3, liquid_Percentage, TFT_COLOR_FOREGROUND);

    // Draw percentage
    _tft->fillRect(x - 10, yTop - 15, 27, 20, TFT_COLOR_BACKGROUND);
    _tft->setTextColor(color);
    DrawCenteredString(String(liquid_Percentage), x + 3, yTop - 5);
  }
}

//===============================================================
// Clears the difference from a bar bottle to the next bottle
//===============================================================
void DisplayDriver::ClearBarBottle(BarBottle lastDraw_barBottle, BarBottle barBottle, int16_t x0, int16_t y, uint16_t clearColor)
{
  if (_imagesAvailable != IMAGE_SUCCESS ||
    lastDraw_barBottle == eEmpty)
  {
    return;
  }

  // Determine image pointers
  SPIFFSImage* barBottlePointerLast = GetBarBottlePointer(lastDraw_barBottle);
  SPIFFSImage* barBottlePointerNew = GetBarBottlePointer(barBottle);

  // Clear difference from last to new image
  int16_t xLast = x0 - barBottlePointerLast->Width() / 2;
  int16_t xNew = x0 - barBottlePointerNew->Width() / 2;
  barBottlePointerLast->ClearDiff(xLast, y, xNew, y, barBottlePointerNew, _tft, TFT_TRANSPARENCY_COLOR, clearColor);
}

//===============================================================
// Draws a bar bottle
//===============================================================
void DisplayDriver::DrawBarBottle(BarBottle barBottle, int16_t x0, int16_t y)
{
  if (_imagesAvailable != IMAGE_SUCCESS)
  {
    return;
  }
  
  // Determine correct pointer
  SPIFFSImage* barBottlePointer = GetBarBottlePointer(barBottle);

  // Draw bottle
  int16_t x = x0 - barBottlePointer->Width() / 2;
  barBottlePointer->Draw(x, y, _tft, TFT_TRANSPARENCY_COLOR, TFT_COLOR_BACKGROUND, barBottle == eEmpty); // Use red wine bottle for empty selection (draw as shadow -> black)
}

//===============================================================
// Draws a selection around a bar bottle
//===============================================================
void DisplayDriver::SelectBarBottle(BarBottle barBottle, int16_t x0, int16_t y, uint16_t color)
{
  if (_imagesAvailable != IMAGE_SUCCESS)
  {
    return;
  }

  // Determine correct pointer
  SPIFFSImage* barBottlePointer = GetBarBottlePointer(barBottle);

  // Draw selection shadow with move function
  int16_t selectionWidth = 3;
  int16_t x = x0 - barBottlePointer->Width() / 2;
  barBottlePointer->Move(x - selectionWidth, y, x, y, _tft, color, TFT_TRANSPARENCY_COLOR, true);
  barBottlePointer->Move(x + selectionWidth, y, x, y, _tft, color, TFT_TRANSPARENCY_COLOR, true);
  barBottlePointer->Move(x, y - selectionWidth, x, y, _tft, color, TFT_TRANSPARENCY_COLOR, true);
}

//===============================================================
// Returns a pointer to the requested bar bottle image
//===============================================================
SPIFFSImage* DisplayDriver::GetBarBottlePointer(BarBottle barBottle)
{
  switch (barBottle)
  {
    case eWhiteWine:
      return _imageBottleWhiteWine;
    case eRoseWine:
      return _imageBottleRoseWine;
    case eSparklingWater:
      return _imageBottleSparklingWater;
    case eRedWine:
    case eEmpty:
    default:
      return _imageBottle;
  }
}

//===============================================================
// Draws a string centered
//===============================================================
void DisplayDriver::DrawCenteredString(const String &text, int16_t x, int16_t y, bool underlined, bool backGround, uint16_t lineColor, uint16_t backGroundColor)
{
  // Get text bounds
  int16_t x1, y1;
  uint16_t w, h;
  _tft->getTextBounds(text, x, y, &x1, &y1, &w, &h);

  // Calculate cursor position
  int16_t x_text = x - w / 2;
  int16_t y_text = y + h / 2;
  _tft->setCursor(x_text, y_text);

  // Draw background if active
  if (backGround)
  {
    _tft->fillRect(x_text - 2, y - h /2, w + 4, h + 4, backGroundColor);
  }

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

  dtostrf(value, mainPlaces, decimalPlaces, _output);
  returnValue += String(_output);

  return returnValue;
}
