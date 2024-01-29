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
  IMAGE_SUCCESS,            // Successful load
  IMAGE_ERR_FILE_NOT_FOUND, // Could not open file
  IMAGE_ERR_FORMAT,         // Not a supported image format
  IMAGE_ERR_MALLOC          // Could not allocate image
};


//===============================================================
// SPIFFS image class
//===============================================================
class SPIFFS_Image
{
  public:
    // Constructor
    SPIFFS_Image();
    
    // Destructor
    ~SPIFFS_Image();
    
    // Draws the canvas on the tft
    void Draw(int16_t x, int16_t y, Adafruit_SPITFT *tft, uint16_t transparencyColor);
    
    // Canvas which stores the pixel data
    GFXcanvas16* Canvas16;

  private:
    // Free/deinitializes variables
    void Dealloc();      

    // Loading occurs here
    friend class SPIFFS_ImageReader;
};


//===============================================================
// SPIFFS image reader class
//===============================================================
class SPIFFS_ImageReader
{
  public:
    // Constructor
    SPIFFS_ImageReader();
    
    // Destructor
    ~SPIFFS_ImageReader();

    // Loads BMP image file from SPIFFS into RAM
    ImageReturnCode LoadBMP(const char *filename, SPIFFS_Image *img);

  private:
    File file;

    // Reads a little-endian 16-bit
    uint16_t ReadLE16();

    // Reads a little-endian 32-bit
    uint32_t ReadLE32();
};

#endif
