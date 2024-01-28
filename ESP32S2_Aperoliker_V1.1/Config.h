/**
 * Includes all config defines
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

#ifndef CONFIG_H
#define CONFIG_H

//===============================================================
// Includes
//===============================================================
#include <Arduino.h>
#include <Adafruit_ST77xx.h>


//===============================================================
// Defines
//===============================================================

// Define one of these or none for generic colors
#define APEROLIKER
//#define HUGOLIKER

//===============================================================

#if defined(APEROLIKER)

// Draw setting
const String defaultHeader = "Aperoliker V1.1";
const String liquid1String = "Aperol";            // Should not exceed 10 characters
const String liquid2String = "Soda";              // Should not exceed 10 characters
const String liquid3String = "Prosecco";          // Should not exceed 10 characters

// Color defines
#define TFT_COLOR_STARTPAGE               ST77XX_ORANGE
#define TFT_COLOR_STARTPAGE_FOREGROUND    0xDF9E
#define TFT_COLOR_STARTPAGE_BACKGROUND    0xA6DC
#define TFT_COLOR_TEXT_HEADER             ST77XX_ORANGE
#define TFT_COLOR_TEXT_BODY               ST77XX_WHITE
#define TFT_COLOR_INFOBOX_BORDER          ST77XX_ORANGE
#define TFT_COLOR_INFOBOX_FOREGROUND      ST77XX_ORANGE
#define TFT_COLOR_INFOBOX_BACKGROUND      ST77XX_WHITE
#define TFT_COLOR_MENU_SELECTOR           ST77XX_ORANGE
#define TFT_COLOR_LIQUID_1                ST77XX_ORANGE
#define TFT_COLOR_LIQUID_2                0x0F1F
#define TFT_COLOR_LIQUID_3                0x0390
#define TFT_COLOR_FOREGROUND              ST77XX_WHITE
#define TFT_COLOR_BACKGROUND              ST77XX_BLACK

// Startup image
const String startupImageLogo = "/LogoAperoliker.bmp";
const String startupImageGlass = "/GlassAperoliker.bmp";
const String startupImageBottle = "/BottleAperoliker.bmp";

#define TFT_TRANSPARENCY_COLOR            0x07E0
#define TFT_LOGO_POS_X                    0
#define TFT_LOGO_POS_Y                    25
#define TFT_GLASS_POS_X                   140
#define TFT_GLASS_POS_Y                   85
#define TFT_BOTTLE_POS_X                  40
#define TFT_BOTTLE_POS_Y                  5

#elif defined(HUGOLIKER)

// Draw setting
const String defaultHeader = "Hugoliker V1.1";
const String liquid1String = "Syrup";             // Should not exceed 10 characters
const String liquid2String = "Soda";              // Should not exceed 10 characters
const String liquid3String = "Prosecco";          // Should not exceed 10 characters

// Color defines
#define TFT_COLOR_STARTPAGE               ST77XX_YELLOW
#define TFT_COLOR_STARTPAGE_FOREGROUND    0xDF9E
#define TFT_COLOR_STARTPAGE_BACKGROUND    0xA6DC
#define TFT_COLOR_TEXT_HEADER             ST77XX_YELLOW
#define TFT_COLOR_TEXT_BODY               ST77XX_WHITE
#define TFT_COLOR_INFOBOX_BORDER          ST77XX_YELLOW
#define TFT_COLOR_INFOBOX_FOREGROUND      ST77XX_BLACK
#define TFT_COLOR_INFOBOX_BACKGROUND      ST77XX_WHITE
#define TFT_COLOR_MENU_SELECTOR           ST77XX_YELLOW
#define TFT_COLOR_LIQUID_1                ST77XX_YELLOW
#define TFT_COLOR_LIQUID_2                ST77XX_CYAN
#define TFT_COLOR_LIQUID_3                0x0390
#define TFT_COLOR_FOREGROUND              ST77XX_WHITE
#define TFT_COLOR_BACKGROUND              ST77XX_BLACK

// Startup image
const String startupImageLogo = "/LogoHugoliker.bmp";
const String startupImageGlass = "/GlassHugoliker.bmp";
const String startupImageBottle = "/BottleHugoliker.bmp";

#define TFT_TRANSPARENCY_COLOR            0x07E0
#define TFT_LOGO_POS_X                    0
#define TFT_LOGO_POS_Y                    25
#define TFT_GLASS_POS_X                   140
#define TFT_GLASS_POS_Y                   85
#define TFT_BOTTLE_POS_X                  40
#define TFT_BOTTLE_POS_Y                  5

#else

// Draw setting
const String defaultHeader = "Mixer V1.1";
const String liquid1String = "Liquid 1";          // Should not exceed 10 characters
const String liquid2String = "Liquid 2";          // Should not exceed 10 characters
const String liquid3String = "Liquid 3";          // Should not exceed 10 characters

// Color defines
#define TFT_COLOR_STARTPAGE               ST77XX_RED
#define TFT_COLOR_STARTPAGE_FOREGROUND    ST77XX_BLACK
#define TFT_COLOR_STARTPAGE_BACKGROUND    ST77XX_WHITE
#define TFT_COLOR_TEXT_HEADER             ST77XX_RED
#define TFT_COLOR_TEXT_BODY               ST77XX_WHITE
#define TFT_COLOR_INFOBOX_BORDER          ST77XX_RED
#define TFT_COLOR_INFOBOX_FOREGROUND      ST77XX_RED
#define TFT_COLOR_INFOBOX_BACKGROUND      ST77XX_WHITE
#define TFT_COLOR_MENU_SELECTOR           ST77XX_RED
#define TFT_COLOR_LIQUID_1                ST77XX_RED
#define TFT_COLOR_LIQUID_2                ST77XX_GREEN
#define TFT_COLOR_LIQUID_3                ST77XX_BLUE
#define TFT_COLOR_FOREGROUND              ST77XX_WHITE
#define TFT_COLOR_BACKGROUND              ST77XX_BLACK

// Startup image
const String startupImageLogo = "/LogoAperoliker.bmp";
const String startupImageGlass = "/GlassAperoliker.bmp";
const String startupImageBottle = "/BottleAperoliker.bmp";

#define TFT_TRANSPARENCY_COLOR            0x07E0
#define TFT_LOGO_POS_X                    0
#define TFT_LOGO_POS_Y                    25
#define TFT_GLASS_POS_X                   140
#define TFT_GLASS_POS_Y                   85
#define TFT_BOTTLE_POS_X                  40
#define TFT_BOTTLE_POS_Y                  5

#endif

#endif
