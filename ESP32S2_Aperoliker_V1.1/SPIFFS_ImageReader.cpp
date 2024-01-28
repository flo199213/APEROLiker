/**
 * Includes all spiffs image functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

#include "SPIFFS_ImageReader.h"

//===============================================================
// Defines
//===============================================================
#define BUFPIXELS 200


//===============================================================
// Constructor
//===============================================================
SPIFFS_ImageReader::SPIFFS_ImageReader()
{
}

//===============================================================
// Destructor
//===============================================================
SPIFFS_ImageReader::~SPIFFS_ImageReader()
{
  if (file)
  {
    file.close();
  }
}

//===============================================================
// Draws an image on the tft
//===============================================================
ImageReturnCode SPIFFS_ImageReader::drawBMP(const char *filename, Adafruit_SPITFT *tft, int16_t x, int16_t y, int32_t transparencyColor)
{
  uint32_t offset;                            // Start of image data in file
  uint32_t headerSize;                        // Indicates BMP version
  int16_t bmpWidth;                           // BMP width in pixels
  int16_t bmpHeight;                          // BMP height in pixels
  uint8_t planes;                             // BMP planes
  uint8_t depth;                              // BMP bit depth
  uint32_t compression = 0;                   // BMP compression mode
  uint32_t rowSize;                           // >bmpWidth if scanline padding
  uint8_t sdbuf[3 * BUFPIXELS] = {};          // BMP read buf (R+G+B/pixel)
  uint16_t srcidx = sizeof sdbuf;
  bool flip = true;                           // BMP is stored bottom-to-top
  uint32_t bmpPos = 0;                        // Next pixel position in file
  int16_t row, col;                           // Current pixel pos.
  uint8_t r, g, b;                            // Current pixel color

  // If BMP is being drawn off the right or bottom edge of the screen,
  // nothing to do here. NOT an error, just a trivial clip operation.
  if (x >= tft->width() || y >= tft->height())
  {
    return IMAGE_SUCCESS;
  }
  
  // Open requested file on SD card
  if (!(file = SPIFFS.open(filename, FILE_READ)))
  {
    return IMAGE_ERR_FILE_NOT_FOUND;
  }

  // Parse BMP header. 0x4D42 (ASCII 'BM') is the Windows BMP signature.
  // There are other values possible in a .BMP file but these are super
  // esoteric (e.g. OS/2 struct bitmap array) and NOT supported here!
  // BMP signature
  if (ReadLE16() != 0x4D42)
  {
    file.close();
    return IMAGE_ERR_FORMAT;
  }

  (void)ReadLE32();         // Read & ignore file size
  (void)ReadLE32();         // Read & ignore creator bytes
  offset = ReadLE32();      // Start of image data
  
  // Read DIB header
  headerSize = ReadLE32();
  bmpWidth = ReadLE32();
  bmpHeight = ReadLE32();

  // If bmpHeight is negative, image is in top-down order.
  // This is not canon but has been observed in the wild.
  if (bmpHeight < 0)
  {
    bmpHeight = -bmpHeight;
    flip = false;
  }
  planes = ReadLE16();
  depth = ReadLE16(); // Bits per pixel

  if (depth != 24)
  {
    file.close();
    return IMAGE_ERR_FORMAT;
  }

  // Compression mode is present in later BMP versions (default = none)
  if (headerSize > 12)
  {
    compression = ReadLE32();
    (void)ReadLE32();    // Raw bitmap data size; ignore
    (void)ReadLE32();    // Horizontal resolution, ignore
    (void)ReadLE32();    // Vertical resolution, ignore
    (void)ReadLE32();    // Number of colors in palette, ignore
    (void)ReadLE32();    // Number of colors used, ignore
    // File position should now be at start of palette (if present)
  }

  // Only uncompressed is handled
  if (planes != 1 || compression != 0)
  {
    file.close();
    return IMAGE_ERR_FORMAT;
  }
  
  // BMP rows are padded (if needed) to 4-byte boundary
  rowSize = ((depth * bmpWidth + 31) / 32) * 4;
          
  // For each scanline...
  for (row = 0; row < bmpHeight; row++)
  {
    yield(); // Keep ESP happy

    // Seek to start of scan line.  It might seem labor-intensive
    // to be doing this on every line, but this method covers a
    // lot of gritty details like cropping, flip and scanline
    // padding. Also, the seek only takes place if the file
    // position actually needs to change (avoids a lot of cluster
    // math in SD library).
    if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
    {
      bmpPos = offset + (bmpHeight - 1 - row) * rowSize;
    }
    else // Bitmap is stored top-to-bottom
    {
      bmpPos = offset + row * rowSize;
    }
    
    // Need seek?
    if (file.position() != bmpPos)
    {
      file.seek(bmpPos);

      // Force buffer reload
      srcidx = sizeof sdbuf;
    }
    
    // For each pixel...
    for (col = 0; col < bmpWidth; col++)
    {
      // Time to load more?
      if (srcidx >= sizeof sdbuf)
      {
        // Read file from SPIFFS
        file.read(sdbuf, sizeof sdbuf);

        // Reset bmp buf index
        srcidx = 0;
      }

      // Convert each pixel from BMP to 565 format
      b = sdbuf[srcidx++];
      g = sdbuf[srcidx++];
      r = sdbuf[srcidx++];
      uint16_t currentPixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
      
      if (transparencyColor == -1 || currentPixel != (uint16_t)transparencyColor)
      {
        tft->drawPixel(x + col, y + row, currentPixel);
      }
      
    } // end column loop
  } // end row loop

  file.close();
  return IMAGE_SUCCESS;
}

//===============================================================
// Reads a little-endian 16-bit unsigned value from currently-
// open File, converting if necessary to the microcontroller's
// native endianism. (BMP files use little-endian values.)
//===============================================================
uint16_t SPIFFS_ImageReader::ReadLE16()
{
#if !defined(ESP32) && !defined(ESP8266) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  // Read directly into result -- BMP data and variable both little-endian.
  uint16_t result;
  file.read(&result, sizeof result);
  return result;
#else
  // Big-endian or unknown. Byte-by-byte read will perform reversal if needed.
  return file.read() | ((uint16_t)file.read() << 8);
#endif
}

//===============================================================
// Reads a little-endian 32-bit unsigned value from currently-
// open File, converting if necessary to the microcontroller's
// native endianism. (BMP files use little-endian values.)
//===============================================================
uint32_t SPIFFS_ImageReader::ReadLE32()
{
#if !defined(ESP32) && !defined(ESP8266) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  // Read directly into result -- BMP data and variable both little-endian.
  uint32_t result;
  file.read(&result, sizeof result);
  return result;
#else
  // Big-endian or unknown. Byte-by-byte read will perform reversal if needed.
  return file.read() | ((uint32_t)file.read() << 8) | ((uint32_t)file.read() << 16) | ((uint32_t)file.read() << 24);
#endif
}

//===============================================================
// Prints 
//===============================================================
void SPIFFS_ImageReader::printStatus(ImageReturnCode stat, Stream &stream)
{
  if (stat == IMAGE_SUCCESS)
  {
    stream.println(F("Success!"));
  }
  else if (stat == IMAGE_ERR_FILE_NOT_FOUND)
  {
    stream.println(F("File not found."));
  }
  else if (stat == IMAGE_ERR_FORMAT)
  {
    stream.println(F("Not a supported BMP variant."));
  }
}
