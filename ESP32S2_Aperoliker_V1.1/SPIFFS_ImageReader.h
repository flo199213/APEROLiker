/**
 * Includes all spiffs image functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */
 
#ifndef SPIFFS_IMAGEREADER_H
#define SPIFFS_IMAGEREADER_H

//===============================================================
// Inlcudes
//===============================================================
#include <SPIFFS.h>
#include <Adafruit_SPITFT.h>

//===============================================================
// Enums
//===============================================================
enum ImageReturnCode
{
  IMAGE_SUCCESS,            // Successful load (or image clipped off screen)
  IMAGE_ERR_FILE_NOT_FOUND, // Could not open file
  IMAGE_ERR_FORMAT          // Not a supported image format
};


//===============================================================
// SPIFFS image reader class
//===============================================================
class SPIFFS_ImageReader
{
  public:
    SPIFFS_ImageReader();
    ~SPIFFS_ImageReader();
    ImageReturnCode drawBMP(const char *filename, Adafruit_SPITFT *tft, int16_t x, int16_t y, int32_t transparencyColor = -1);
    void printStatus(ImageReturnCode stat, Stream &stream = Serial);

  private:
    File file;
    uint16_t ReadLE16();
    uint32_t ReadLE32();
};

#endif
