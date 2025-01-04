/**
 * Includes all spiffs editor functions
 *
 * @author    Florian Staeblein
 * @date      2024/01/28
 * @copyright © 2024 Florian Staeblein
 */

//===============================================================
// Includes
//=============================================================== 
#include "SPIFFSEditor.h"

//===============================================================
// Defines
//=============================================================== 
#define edit_html_gz_len              4249

//===============================================================
// https://mischianti.org/online-converter-file-to-cpp-gzip-byte-array-3/
// File: edit.html.gz, Size: 4249
//=============================================================== 
const uint8_t edit_html_gz[] PROGMEM =
{  
  0x1F, 0x8B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xCD, 0x5B, 0xEB, 0x5E, 0xDB, 0xC8, 
  0x92, 0xFF, 0xCE, 0x53, 0x74, 0x94, 0x9D, 0x41, 0x1E, 0x7C, 0xC3, 0x10, 0x86, 0x98, 0x4B, 0xC6, 
  0x36, 0xE6, 0x12, 0x08, 0x37, 0x9B, 0x24, 0x24, 0x27, 0xE7, 0xFC, 0xDA, 0x52, 0xDB, 0x56, 0x90, 
  0x25, 0x45, 0x6A, 0x63, 0x3B, 0x0C, 0x6F, 0xB3, 0x5F, 0xF6, 0x35, 0x76, 0x5F, 0x6C, 0xAB, 0xFA, 
  0xA2, 0x8B, 0x2D, 0x43, 0x92, 0x3D, 0x1F, 0x36, 0xCC, 0x04, 0xAB, 0xBB, 0xBA, 0xBA, 0xBA, 0x2E, 
  0xFF, 0xAA, 0x6A, 0x39, 0xBB, 0x2F, 0x4A, 0xA5, 0xEE, 0xD0, 0x89, 0x08, 0xFC, 0xC7, 0x87, 0x8C, 
  0x04, 0x2E, 0x75, 0x3C, 0x32, 0xE4, 0x23, 0x97, 0x44, 0xFE, 0x38, 0xB4, 0x18, 0xF1, 0xFB, 0x62, 
  0x62, 0xC8, 0xA6, 0x84, 0x79, 0x96, 0x6F, 0x33, 0x9B, 0xB4, 0x6D, 0x87, 0xFB, 0x61, 0xE9, 0x92, 
  0x0E, 0x18, 0x61, 0xA3, 0x1E, 0xB3, 0x71, 0x10, 0x96, 0x75, 0x2E, 0x4F, 0x0E, 0x0F, 0x3B, 0x72, 
  0xB6, 0x6C, 0x05, 0x01, 0x29, 0x95, 0xF6, 0x57, 0x76, 0x5F, 0x1C, 0x5C, 0xB4, 0xBA, 0xB7, 0x97, 
  0x6D, 0xC1, 0x15, 0x9E, 0x05, 0x73, 0x97, 0x7A, 0x83, 0x3D, 0x83, 0x79, 0xC6, 0xFE, 0x0A, 0x21, 
  0xBB, 0x43, 0x46, 0x6D, 0xFC, 0x00, 0x1F, 0x47, 0x8C, 0x53, 0x62, 0x0D, 0x69, 0x18, 0x31, 0xBE, 
  0x67, 0xDC, 0x74, 0x0F, 0x4B, 0xDB, 0x86, 0x9A, 0xE2, 0x0E, 0x77, 0xD9, 0x7E, 0xBB, 0x73, 0xA9, 
  0x04, 0xD8, 0xAD, 0xC8, 0x91, 0xD4, 0x42, 0x8F, 0x8E, 0xD8, 0x9E, 0x61, 0xB3, 0xC8, 0x0A, 0x9D, 
  0x80, 0x3B, 0xBE, 0x67, 0x10, 0xCB, 0xF7, 0x38, 0xF3, 0x80, 0x57, 0xCB, 0xB7, 0xEE, 0x38, 0x75, 
  0xDC, 0x91, 0x33, 0x65, 0xA1, 0x92, 0x55, 0x71, 0x22, 0x25, 0xD2, 0xF2, 0x83, 0x59, 0xE8, 0x0C, 
  0x86, 0x9C, 0xFC, 0xF7, 0x7F, 0x91, 0x5A, 0xB5, 0xB6, 0x49, 0x0E, 0x5D, 0x3F, 0x74, 0x28, 0x9C, 
  0x8A, 0xFF, 0xCF, 0x7F, 0xF6, 0x5C, 0xE6, 0x78, 0xC6, 0xE2, 0x4E, 0xF7, 0x0E, 0x9B, 0x04, 0x7E, 
  0xC8, 0x53, 0xDB, 0x8C, 0x23, 0x16, 0x96, 0x22, 0x8B, 0xBA, 0x14, 0x16, 0xED, 0x79, 0x7E, 0x11, 
  0x34, 0xE3, 0x70, 0x87, 0xBA, 0x62, 0x90, 0xED, 0xAD, 0x17, 0xC9, 0x88, 0x4E, 0x9D, 0xD1, 0x78, 
  0x94, 0x1A, 0x00, 0x92, 0xCC, 0xC0, 0xC4, 0xB1, 0xF9, 0x70, 0xCF, 0x66, 0xF7, 0x8E, 0xC5, 0x4A, 
  0xE2, 0xA1, 0x08, 0x16, 0x40, 0xF1, 0xF4, 0xA0, 0x7C, 0x2A, 0x12, 0x4E, 0xC3, 0x01, 0xE3, 0x25, 
  0x9B, 0x79, 0x91, 0xC3, 0x67, 0x76, 0xE0, 0x68, 0x02, 0xF8, 0x68, 0x54, 0xA4, 0xC4, 0x52, 0xEC, 
  0x88, 0xCF, 0x5C, 0x46, 0xF8, 0x2C, 0x00, 0xB9, 0x39, 0x9B, 0xF2, 0x8A, 0x15, 0x45, 0x06, 0x19, 
  0x31, 0xDB, 0xA1, 0x7B, 0x06, 0x28, 0x8C, 0x09, 0x6B, 0x94, 0xC5, 0x41, 0xA6, 0xFC, 0x1D, 0xF3, 
  0xC6, 0x2B, 0x0F, 0xB0, 0xF2, 0x7B, 0xC9, 0xF1, 0x6C, 0x36, 0xAD, 0x93, 0x8D, 0x6A, 0x75, 0x07, 
  0x9E, 0x03, 0x1F, 0x36, 0x02, 0xCD, 0xD6, 0x09, 0xED, 0x45, 0xBE, 0x3B, 0xE6, 0x0C, 0x47, 0x5D, 
  0xD6, 0xE7, 0x75, 0xF2, 0x2A, 0x98, 0xE2, 0x43, 0xCF, 0x0F, 0x6D, 0x16, 0xD6, 0xC9, 0x7A, 0x30, 
  0x05, 0x2F, 0x72, 0x1D, 0x9B, 0xBC, 0xDC, 0xDC, 0xDC, 0x14, 0x33, 0xD4, 0xBA, 0x1B, 0x84, 0xFE, 
  0xD8, 0xB3, 0x4B, 0x96, 0x0F, 0xFA, 0xAD, 0x93, 0x97, 0x87, 0xAF, 0xF0, 0x07, 0x27, 0x6D, 0x27, 
  0x02, 0xF7, 0x9B, 0xD5, 0x89, 0xE7, 0x7B, 0x4C, 0xF2, 0x99, 0x96, 0xA2, 0x21, 0xB5, 0xFD, 0x49, 
  0x9D, 0x54, 0xE1, 0x67, 0xBD, 0x0A, 0x0C, 0xC3, 0x41, 0x8F, 0x9A, 0xA4, 0x5A, 0x54, 0xFF, 0x95, 
  0x37, 0x49, 0x01, 0x69, 0xFB, 0x20, 0x76, 0x29, 0x72, 0xBE, 0x33, 0xD8, 0xB6, 0x26, 0xC5, 0x10, 
  0x43, 0x7D, 0x3A, 0x72, 0x5C, 0xE0, 0x19, 0x51, 0x2F, 0x2A, 0x81, 0x69, 0x9C, 0x7E, 0x3C, 0x35, 
  0x11, 0x2A, 0xAC, 0xF7, 0x7C, 0xD7, 0xDE, 0x59, 0x79, 0x5C, 0xC9, 0x1C, 0x9D, 0x8C, 0x5D, 0x71, 
  0x7A, 0xD7, 0x89, 0x80, 0x2D, 0x6A, 0x2E, 0x11, 0x8B, 0xFB, 0x01, 0xC8, 0x93, 0x9C, 0x5A, 0x7C, 
  0x1C, 0x81, 0x1D, 0x1C, 0x4F, 0x3D, 0x04, 0xD4, 0xB6, 0x1D, 0x6F, 0x20, 0x9E, 0xE6, 0x19, 0xBB, 
  0x8E, 0x60, 0x9C, 0xA8, 0x31, 0x64, 0x2E, 0xE5, 0xCE, 0xBD, 0x60, 0x0D, 0x8E, 0x20, 0xCD, 0x5D, 
  0x27, 0x5B, 0x55, 0x79, 0x0A, 0x6B, 0x1C, 0x46, 0xA8, 0xA8, 0xC0, 0x77, 0x80, 0x49, 0xB8, 0xC8, 
  0x30, 0x0A, 0xA8, 0x27, 0x58, 0x6A, 0x8D, 0x2A, 0x5D, 0xC7, 0xEA, 0x74, 0x3C, 0xD7, 0xF1, 0x58, 
  0xA9, 0xE7, 0x42, 0x00, 0x64, 0xA4, 0xDB, 0xC2, 0x1D, 0x16, 0xE5, 0xAB, 0x0F, 0xFD, 0x7B, 0x16, 
  0x0A, 0x96, 0x89, 0xBD, 0x34, 0xDF, 0x65, 0xE4, 0x8B, 0x62, 0xB4, 0xDB, 0x6D, 0x49, 0xCE, 0xEF, 
  0x63, 0x75, 0x66, 0xB4, 0x94, 0xA7, 0xDB, 0x58, 0xB6, 0x6D, 0x2D, 0x1B, 0xAE, 0xD6, 0x3A, 0x4B, 
  0xA9, 0xF5, 0x59, 0x5E, 0x7A, 0xAD, 0xE3, 0x05, 0x63, 0x3E, 0xA7, 0xF2, 0xB4, 0xE7, 0xFA, 0x01, 
  0xB5, 0x20, 0x72, 0x62, 0x5B, 0xC1, 0x1A, 0x41, 0x8D, 0x2E, 0x82, 0xBC, 0xC2, 0x11, 0x75, 0x85, 
  0x4B, 0x91, 0xF7, 0x2C, 0xB4, 0xA9, 0x47, 0x8B, 0xA4, 0x01, 0xB8, 0xE0, 0x16, 0x49, 0x27, 0xE3, 
  0x52, 0xA5, 0x91, 0xFF, 0xBD, 0x24, 0x83, 0x9F, 0xB9, 0xCC, 0xE2, 0xC9, 0x99, 0xC0, 0xD1, 0x7A, 
  0x77, 0x0E, 0xCF, 0x9F, 0xCC, 0x1D, 0x9C, 0xB3, 0xA4, 0x30, 0x9F, 0x0C, 0x78, 0xF0, 0xEE, 0xAD, 
  0xB4, 0x66, 0x62, 0xAD, 0x4B, 0x6D, 0x94, 0x7A, 0x3E, 0xE7, 0xFE, 0xA8, 0xAE, 0x02, 0x31, 0x51, 
  0x98, 0x8C, 0x9F, 0xED, 0x25, 0x2E, 0xF5, 0x84, 0xB3, 0x64, 0xB7, 0x25, 0x04, 0x4C, 0xCD, 0x1D, 
  0x80, 0xA9, 0x12, 0x75, 0x9D, 0x01, 0xA8, 0x72, 0xE4, 0xD8, 0xB6, 0xCB, 0x76, 0xE6, 0x7C, 0x65, 
  0x1C, 0xBA, 0xE6, 0xAA, 0x4D, 0x39, 0xAD, 0x3B, 0x23, 0xC8, 0x12, 0x95, 0xC0, 0x1B, 0xEC, 0xF4, 
  0x68, 0xC4, 0xB6, 0x36, 0x8B, 0xCE, 0xFB, 0xE6, 0xC5, 0xF5, 0xA4, 0x7A, 0x7A, 0x34, 0xF0, 0x1B, 
  0xF0, 0xE7, 0xBC, 0x73, 0x33, 0x6C, 0xDF, 0x0C, 0xE0, 0x53, 0x13, 0x1F, 0x1B, 0x57, 0xAD, 0xC6, 
  0x15, 0xFC, 0x6A, 0xAD, 0xAF, 0x7D, 0xED, 0x7F, 0xC3, 0x81, 0xE6, 0x91, 0xDD, 0xEC, 0xDE, 0xB4, 
  0x1B, 0x8D, 0xD3, 0xCA, 0xC9, 0xF9, 0xE4, 0xC3, 0xE9, 0x16, 0x4E, 0x37, 0xDD, 0xEA, 0xF5, 0xFB, 
  0x61, 0xF5, 0xA6, 0xF6, 0x7A, 0x64, 0x1F, 0xDB, 0x43, 0x6B, 0x74, 0xD3, 0xB8, 0xFA, 0x70, 0x7D, 
  0x7F, 0x3B, 0xBA, 0x19, 0x74, 0x3E, 0xAC, 0x0F, 0x3F, 0xD5, 0xDE, 0x77, 0x3E, 0x7D, 0x38, 0xBC, 
  0x63, 0x1F, 0x8F, 0xDF, 0x7E, 0xEA, 0x4E, 0x60, 0xC1, 0x81, 0xDF, 0xB9, 0xB9, 0x6E, 0xBE, 0x6F, 
  0x0E, 0x3E, 0x35, 0xAD, 0xF6, 0xB4, 0xE7, 0x9E, 0x37, 0x8F, 0x1A, 0xBD, 0x46, 0xCD, 0x62, 0x6C, 
  0xD0, 0xBD, 0x6E, 0x8E, 0x4F, 0x4E, 0x2F, 0x06, 0x8E, 0x33, 0xEC, 0x7C, 0x3A, 0xEF, 0x5A, 0xAD, 
  0x57, 0x67, 0x37, 0xC7, 0x0D, 0x67, 0x78, 0xFE, 0xF6, 0xBA, 0x7A, 0x77, 0x74, 0xDA, 0x3A, 0xB0, 
  0x6E, 0xDF, 0xDE, 0x6E, 0x1D, 0x6C, 0x54, 0xFE, 0xFC, 0xF3, 0x9D, 0x7D, 0xE1, 0x78, 0xDD, 0xFB, 
  0xEF, 0x8D, 0x41, 0x6B, 0xB2, 0x3D, 0x8B, 0xBA, 0xC3, 0x93, 0x7B, 0xAF, 0xF2, 0xDE, 0xFF, 0x7A, 
  0x32, 0x7B, 0x07, 0xFF, 0x5F, 0x5E, 0xAE, 0xF5, 0x3A, 0xEB, 0xD1, 0xCD, 0xD5, 0xC9, 0xFB, 0x5A, 
  0xF4, 0xFA, 0x55, 0xD0, 0x3C, 0x38, 0xB8, 0x1F, 0xF5, 0x2E, 0x2B, 0x23, 0xFB, 0xAE, 0xCF, 0xB7, 
  0x37, 0x78, 0x70, 0x3B, 0x18, 0x7F, 0xFA, 0xF6, 0xEA, 0xED, 0xB0, 0x72, 0xC1, 0xE8, 0xED, 0x70, 
  0x6D, 0xF6, 0x7D, 0xB6, 0x3D, 0xEC, 0x1E, 0xDF, 0x9F, 0xBB, 0x74, 0x7A, 0x7E, 0x6E, 0x7D, 0x1F, 
  0xAD, 0xB9, 0xF4, 0xF5, 0x45, 0xD7, 0xA5, 0xE1, 0xFA, 0x8D, 0xDD, 0xA8, 0xAC, 0xB5, 0x6A, 0x8D, 
  0x4D, 0x1E, 0x5E, 0xB7, 0xBC, 0x83, 0x8D, 0xAF, 0x9D, 0xED, 0x66, 0x73, 0xDD, 0xEF, 0x7D, 0xAB, 
  0x1D, 0xDD, 0x6D, 0x1D, 0xDD, 0x6C, 0x5D, 0xF5, 0xAE, 0x1A, 0x9D, 0xCD, 0xE6, 0x0D, 0xBD, 0xED, 
  0x5C, 0x35, 0xFA, 0x9B, 0xBD, 0xE1, 0xF0, 0xF4, 0xB4, 0x7B, 0x68, 0x37, 0xBE, 0x87, 0x8D, 0x8B, 
  0x49, 0x63, 0xDA, 0xBE, 0x69, 0x1C, 0xAF, 0x9D, 0xB6, 0xDF, 0x55, 0xD7, 0x3B, 0xB7, 0x1B, 0x83, 
  0x77, 0x5B, 0x93, 0x66, 0xD4, 0x6E, 0x5C, 0x35, 0xAB, 0x83, 0xB7, 0x6B, 0x23, 0xFA, 0xC9, 0x6F, 
  0x6D, 0x0C, 0x4E, 0xB6, 0x9C, 0xCB, 0x5B, 0xDA, 0x38, 0x69, 0xBE, 0x8D, 0x9C, 0xEB, 0xD1, 0xF1, 
  0x4D, 0xB5, 0xD1, 0x38, 0xBB, 0x60, 0x87, 0xAD, 0x0D, 0x7A, 0x5A, 0xB3, 0x3E, 0x80, 0xFE, 0x6F, 
  0x3E, 0xB2, 0x3F, 0xD7, 0x1A, 0x93, 0x8B, 0xAA, 0x6B, 0xBD, 0x66, 0xDD, 0xE3, 0xDB, 0xAE, 0xB0, 
  0x4E, 0xDB, 0x3D, 0xEC, 0xDE, 0x75, 0xC6, 0x57, 0xA3, 0x56, 0x6B, 0xB5, 0x00, 0xEE, 0x5A, 0x0A, 
  0x59, 0xC0, 0x28, 0x9F, 0x83, 0xF6, 0x24, 0xD0, 0x10, 0xB0, 0xAB, 0xF3, 0xFE, 0x9A, 0xC2, 0x17, 
  0x84, 0x11, 0xC8, 0x5D, 0x96, 0x1F, 0x52, 0xB9, 0x00, 0x96, 0xB3, 0x10, 0x3D, 0x50, 0x2C, 0xF9, 
  0x4B, 0x64, 0x27, 0x22, 0x93, 0x13, 0xA1, 0x9E, 0x4D, 0x4C, 0x1D, 0x4C, 0x88, 0x94, 0x2A, 0xD3, 
  0x05, 0x90, 0xD0, 0xDD, 0x92, 0xE0, 0x50, 0xAF, 0x16, 0x04, 0x5F, 0x11, 0xC5, 0x84, 0x3C, 0x88, 
  0xCC, 0xA7, 0x97, 0x50, 0x48, 0xB2, 0x6A, 0x1B, 0x3D, 0x62, 0x7F, 0xA5, 0x16, 0x64, 0xED, 0x12, 
  0xC4, 0xDF, 0x08, 0x7F, 0xCB, 0x40, 0x84, 0xDA, 0xA6, 0x37, 0x1E, 0xF4, 0x9D, 0x29, 0x44, 0x43, 
  0x1F, 0xB3, 0x37, 0x23, 0xEB, 0x11, 0x9E, 0x11, 0x44, 0x22, 0xE4, 0x2F, 0xCD, 0xEF, 0x8E, 0xCD, 
  0xFA, 0x21, 0x94, 0x01, 0xD1, 0x8F, 0xB2, 0x8B, 0x25, 0xEA, 0x87, 0xFE, 0x48, 0x7C, 0x78, 0x88, 
  0xD3, 0xF3, 0x3C, 0xBA, 0xC1, 0x66, 0x72, 0x82, 0xFB, 0x8A, 0x72, 0x19, 0x99, 0x90, 0x0B, 0x24, 
  0x7B, 0x39, 0x0E, 0x5C, 0x9F, 0x82, 0xFA, 0x10, 0x97, 0x3B, 0x1C, 0x0A, 0xA0, 0x27, 0x50, 0x2F, 
  0xC9, 0x61, 0xA1, 0x8C, 0xEF, 0xF9, 0x7C, 0xA6, 0xE2, 0xBE, 0xA6, 0x20, 0x23, 0x03, 0x41, 0xB5, 
  0xCD, 0x0C, 0xBC, 0x94, 0xE4, 0xB2, 0x75, 0x95, 0xB0, 0x72, 0x72, 0x7C, 0xED, 0x75, 0xED, 0x70, 
  0xB3, 0x9A, 0x20, 0xDB, 0xCB, 0xC3, 0x1A, 0xFE, 0x64, 0x33, 0xDE, 0x56, 0x55, 0x7B, 0xCA, 0xCB, 
  0xBE, 0xE3, 0xB2, 0x2E, 0xD8, 0x7C, 0x45, 0x9C, 0xFA, 0x89, 0x13, 0x68, 0xF1, 0x14, 0xEC, 0x2D, 
  0x9C, 0x42, 0x72, 0x5E, 0xAF, 0x56, 0x7F, 0x5B, 0x56, 0x7D, 0x54, 0xF1, 0x47, 0xEE, 0x1A, 0xCD, 
  0x22, 0xCE, 0x40, 0x9E, 0xBE, 0xFF, 0x6B, 0xFB, 0x6A, 0x4D, 0xCA, 0x29, 0x95, 0xC6, 0x37, 0xAB, 
  0xD5, 0x39, 0x2C, 0xDE, 0x5E, 0xAE, 0xA6, 0xAD, 0x57, 0x07, 0x8D, 0xAD, 0x74, 0x05, 0x55, 0x4B, 
  0x2A, 0xA8, 0x44, 0x87, 0x32, 0x33, 0x61, 0xD9, 0xF2, 0x7C, 0x5E, 0xCA, 0x9E, 0xEB, 0x10, 0xDD, 
  0x55, 0x78, 0x85, 0x96, 0xAE, 0xA6, 0xA4, 0xD3, 0xA6, 0xDD, 0xD2, 0xE2, 0x6A, 0x09, 0x74, 0x4A, 
  0x7A, 0xAA, 0xBE, 0x79, 0x29, 0xFD, 0xEE, 0x59, 0xAD, 0x6D, 0x6C, 0xFD, 0x96, 0xD2, 0xD3, 0x26, 
  0x1A, 0x05, 0x71, 0x41, 0xAF, 0x4E, 0x17, 0x9B, 0x60, 0x32, 0x59, 0x6E, 0x6A, 0x39, 0xB6, 0x13, 
  0x4D, 0xF4, 0x5E, 0xE1, 0xCF, 0x0E, 0xA9, 0xFC, 0x41, 0x8E, 0x42, 0x36, 0x23, 0x7F, 0x54, 0x62, 
  0xBA, 0x92, 0xD8, 0x27, 0x45, 0xBB, 0xB1, 0xF9, 0x7A, 0xDB, 0xEE, 0x09, 0xDA, 0xA6, 0x3B, 0x66, 
  0x19, 0x5A, 0x6D, 0xBC, 0x1F, 0x24, 0x0F, 0xA9, 0xED, 0x8C, 0x23, 0x28, 0x72, 0xA5, 0x33, 0x29, 
  0x0D, 0xD6, 0x36, 0xE7, 0x34, 0x18, 0x0F, 0xA4, 0xE0, 0x26, 0x0A, 0xA0, 0x3B, 0xAA, 0x45, 0x22, 
  0x8A, 0x68, 0x18, 0xA3, 0x4A, 0x3A, 0xF1, 0xC6, 0x45, 0xCB, 0x5F, 0x09, 0xAC, 0xE0, 0x32, 0xA1, 
  0x96, 0xEA, 0x6F, 0x10, 0xFF, 0x3C, 0x04, 0xBB, 0xF6, 0xA1, 0x1E, 0x81, 0x92, 0xD1, 0xE7, 0x94, 
  0x33, 0xB3, 0x6A, 0xB3, 0x41, 0x61, 0x47, 0x20, 0x00, 0xBA, 0x78, 0x2E, 0xCD, 0xC6, 0x56, 0x4C, 
  0xF5, 0x28, 0xFB, 0x80, 0x8A, 0x28, 0x93, 0x54, 0x2F, 0x23, 0x1B, 0xA5, 0xFD, 0x15, 0xA7, 0x4F, 
  0x4C, 0x6C, 0x0D, 0xA0, 0xDB, 0xFB, 0xF8, 0xEE, 0xEC, 0x98, 0xF3, 0xE0, 0x9A, 0x7D, 0x1B, 0xB3, 
  0x88, 0x93, 0xBD, 0xBD, 0x3D, 0x62, 0x20, 0x2C, 0x83, 0xCC, 0xCC, 0x36, 0x24, 0xBE, 0xCE, 0xD3, 
  0x90, 0xFE, 0xD8, 0xB3, 0xF0, 0xAC, 0xC4, 0x2C, 0xC4, 0x28, 0xC7, 0xC3, 0x19, 0x88, 0x14, 0x32, 
  0x3E, 0x0E, 0x3D, 0xE2, 0xB1, 0x09, 0x69, 0x58, 0x58, 0xE7, 0x7E, 0xBC, 0xE8, 0x7D, 0x05, 0x54, 
  0x34, 0x8D, 0x77, 0xD1, 0x74, 0xE4, 0xD6, 0xCA, 0xC8, 0xAB, 0xDB, 0xBD, 0x2C, 0x6F, 0x95, 0xAB, 
  0x06, 0xCA, 0x49, 0x2C, 0xCA, 0xAD, 0x21, 0x31, 0x59, 0x81, 0x3C, 0x3C, 0xFE, 0x0A, 0xA3, 0x8D, 
  0x5F, 0x67, 0xE4, 0x58, 0xA1, 0x1F, 0xF9, 0x7D, 0xAE, 0x79, 0x2D, 0xE3, 0x33, 0x0C, 0xFD, 0x89, 
  0x60, 0xD0, 0x0E, 0x43, 0x3F, 0x34, 0x0D, 0xD1, 0x42, 0xF7, 0x60, 0x10, 0xC2, 0x8E, 0xD8, 0x3E, 
  0xD8, 0xCE, 0xF3, 0x39, 0x89, 0xC6, 0x01, 0x36, 0x87, 0x73, 0xDA, 0x2A, 0x1B, 0xA2, 0x61, 0x79, 
  0x14, 0xD6, 0x8E, 0xF5, 0x06, 0x05, 0x19, 0x6F, 0xCE, 0x4E, 0xD9, 0xCC, 0xA4, 0x61, 0x48, 0x67, 
  0x45, 0x02, 0x6E, 0x20, 0x75, 0xAD, 0xC4, 0x15, 0xC3, 0x65, 0x24, 0x33, 0xF5, 0x22, 0x13, 0x62, 
  0xBE, 0x97, 0xE8, 0xFB, 0x1E, 0x5C, 0x6B, 0x0A, 0xB6, 0xA0, 0x9F, 0x61, 0xED, 0x97, 0x1D, 0xF1, 
  0x3C, 0x83, 0xE7, 0x9E, 0x7C, 0x16, 0x34, 0x8A, 0x99, 0x69, 0x4E, 0xC9, 0x2E, 0x99, 0x15, 0xC8, 
  0x1B, 0x52, 0x5A, 0x27, 0x75, 0xF1, 0xBC, 0x2F, 0x9F, 0xF1, 0xB1, 0x5A, 0x90, 0x22, 0x16, 0x84, 
  0x8C, 0x2B, 0xC8, 0xE8, 0x6A, 0xCC, 0xC6, 0xCC, 0x56, 0x47, 0x80, 0x43, 0x66, 0x4D, 0x2E, 0x72, 
  0x39, 0xE8, 0xA0, 0xFC, 0x0D, 0xC9, 0x60, 0xF2, 0xB3, 0xD8, 0x4F, 0x0C, 0x85, 0x63, 0xCF, 0x03, 
  0xB0, 0xC0, 0x15, 0xD4, 0x8D, 0x58, 0x3C, 0x0E, 0x36, 0x1B, 0x82, 0x56, 0x60, 0xDC, 0x1B, 0xBB, 
  0xAE, 0xD8, 0x69, 0x6E, 0x93, 0x72, 0x00, 0x4E, 0xEC, 0xA3, 0x5B, 0x92, 0x3D, 0xB1, 0xC5, 0xBF, 
  0x42, 0x39, 0x55, 0x8F, 0x37, 0x37, 0x61, 0x24, 0xE5, 0x71, 0xD9, 0xFD, 0x78, 0x38, 0x66, 0x3B, 
  0x49, 0x07, 0x8C, 0x3E, 0xFE, 0x02, 0xE8, 0x21, 0xFE, 0x22, 0x4E, 0x3D, 0x0B, 0x9D, 0x5D, 0x5A, 
  0xBE, 0x20, 0x53, 0xAC, 0xCA, 0xB0, 0x52, 0x47, 0x49, 0x76, 0x55, 0xCB, 0x51, 0x0B, 0x7C, 0x48, 
  0xD1, 0xDB, 0x71, 0x9F, 0x14, 0xE3, 0x58, 0x13, 0xF4, 0x2B, 0x9D, 0xB6, 0x7A, 0xE6, 0xB4, 0x48, 
  0xEC, 0x3C, 0x96, 0x89, 0xD4, 0x05, 0x35, 0xAE, 0xE7, 0xA5, 0x70, 0xD3, 0x72, 0xC8, 0xA8, 0x3D, 
  0xEB, 0x60, 0xE0, 0x42, 0xDC, 0x91, 0xCD, 0x42, 0x3C, 0x9D, 0x10, 0x82, 0x7B, 0x59, 0x63, 0xAC, 
  0x2E, 0xCA, 0xD0, 0xF7, 0xB7, 0x65, 0xA1, 0xD1, 0x9C, 0x9D, 0xD8, 0xA6, 0x21, 0x71, 0xD4, 0x28, 
  0x94, 0x45, 0x80, 0x97, 0x15, 0xB2, 0x80, 0xB8, 0x06, 0x82, 0x8B, 0xB1, 0x93, 0xB0, 0x20, 0x76, 
  0x19, 0x4A, 0x78, 0x17, 0x53, 0x10, 0x6C, 0x0A, 0xCA, 0xE0, 0xE3, 0xA8, 0x48, 0x70, 0xFB, 0x28, 
  0xF0, 0xBD, 0x88, 0x75, 0xA1, 0x2C, 0x2B, 0xA4, 0xE9, 0x53, 0x1F, 0x05, 0x52, 0x80, 0x1A, 0xA4, 
  0xA9, 0xCB, 0x2E, 0xF3, 0x06, 0x7C, 0x28, 0x50, 0xA2, 0x5A, 0x48, 0x91, 0xA5, 0x04, 0x16, 0x86, 
  0x81, 0x05, 0x39, 0x8E, 0x10, 0xFF, 0x79, 0xCC, 0xDB, 0x41, 0x2D, 0x78, 0x86, 0xAD, 0x76, 0x8A, 
  0xB4, 0x54, 0xD1, 0xD0, 0xE9, 0x73, 0xB3, 0x90, 0x39, 0x43, 0xB2, 0x85, 0xFE, 0xF4, 0xA8, 0x4C, 
  0xBC, 0xF2, 0x8B, 0x6A, 0x15, 0x4D, 0x92, 0xB1, 0x93, 0x75, 0x11, 0x74, 0x68, 0x43, 0x0D, 0xE2, 
  0x49, 0x40, 0xBA, 0x72, 0x40, 0x01, 0xD0, 0xA3, 0xB4, 0xDF, 0x1D, 0x02, 0x46, 0x1F, 0x40, 0x7F, 
  0x94, 0x75, 0x13, 0x5C, 0x9B, 0xD0, 0xA7, 0x3D, 0x90, 0x81, 0xC2, 0x9E, 0x60, 0x97, 0xE7, 0xC6, 
  0x90, 0x06, 0x88, 0x89, 0x12, 0x41, 0xF4, 0xE3, 0xED, 0x5C, 0xB2, 0x32, 0xDF, 0xFB, 0x02, 0x09, 
  0xF5, 0x46, 0xAE, 0xD3, 0x81, 0x68, 0x6B, 0x7B, 0x72, 0xF7, 0x11, 0xE3, 0x43, 0xDF, 0x96, 0xC4, 
  0x47, 0xED, 0xAE, 0x51, 0x78, 0x63, 0xBC, 0x31, 0xEA, 0x46, 0xCA, 0xBF, 0x12, 0x4D, 0xA3, 0xD8, 
  0xCB, 0xD9, 0x19, 0xBF, 0xE7, 0x2E, 0x12, 0x73, 0xF2, 0xAE, 0xF1, 0xE6, 0xFA, 0xA4, 0xE5, 0x8F, 
  0xC0, 0x27, 0xC1, 0x1A, 0x26, 0x22, 0x22, 0x59, 0x23, 0xC6, 0x9E, 0x01, 0x7F, 0xE7, 0xCC, 0x27, 
  0x07, 0x14, 0x80, 0x17, 0x1B, 0xFF, 0x71, 0x21, 0x94, 0xE7, 0xF1, 0x07, 0x80, 0x3C, 0x8B, 0xD1, 
  0xA6, 0x5A, 0x9C, 0x26, 0x2C, 0xFB, 0x9E, 0x08, 0x4F, 0x0C, 0x17, 0x66, 0x0D, 0xA9, 0x37, 0x40, 
  0xA0, 0x53, 0x21, 0x9F, 0x26, 0x2C, 0xA2, 0xAA, 0x0B, 0x59, 0x0F, 0x58, 0x50, 0x5A, 0xC6, 0x54, 
  0xD9, 0x6D, 0x02, 0xE6, 0xA5, 0xD6, 0x08, 0x6E, 0x65, 0x68, 0xA8, 0xD7, 0x80, 0x31, 0x22, 0x5A, 
  0x7C, 0xAE, 0xCC, 0xAA, 0x88, 0x79, 0xB6, 0x96, 0x3A, 0x71, 0x99, 0x9F, 0xDF, 0x65, 0x6E, 0x0F, 
  0xE9, 0x18, 0x29, 0x4F, 0xC3, 0xC6, 0x22, 0x09, 0xC9, 0xC4, 0xA4, 0x73, 0xC2, 0x70, 0xA5, 0xC8, 
  0x63, 0x86, 0xC1, 0x63, 0x1A, 0x2D, 0x79, 0x3D, 0x5A, 0x42, 0x30, 0x37, 0x8A, 0xC4, 0xA0, 0x41, 
  0xE0, 0x3A, 0x96, 0x28, 0x8F, 0x2A, 0xD3, 0xD2, 0x64, 0x32, 0x29, 0x61, 0xD5, 0x52, 0x02, 0x01, 
  0xD4, 0x25, 0xB3, 0x91, 0x98, 0x6F, 0xE9, 0x71, 0x83, 0xD4, 0x79, 0x1F, 0x8B, 0x2B, 0xC2, 0xB8, 
  0x91, 0xA8, 0x00, 0x33, 0x40, 0xFB, 0x10, 0x5B, 0x22, 0x9D, 0x1F, 0x72, 0x2C, 0x90, 0x0B, 0x50, 
  0x8F, 0xD9, 0xD5, 0xDA, 0x6B, 0x7E, 0xFF, 0x3D, 0x2B, 0x50, 0x0A, 0xB9, 0x77, 0x35, 0x70, 0xE7, 
  0x2A, 0x9E, 0xF6, 0x30, 0x85, 0xE7, 0x48, 0x0E, 0x35, 0x75, 0x4A, 0x70, 0x6D, 0x16, 0x61, 0x12, 
  0xE9, 0xD5, 0x45, 0xA2, 0x41, 0x7B, 0x2E, 0xE3, 0x49, 0xCC, 0x0B, 0xC6, 0xD1, 0xD0, 0x7C, 0xC0, 
  0x05, 0x75, 0xB1, 0x48, 0x72, 0xA8, 0x6B, 0x46, 0x92, 0x47, 0x7D, 0x9E, 0x55, 0x5D, 0x7F, 0x10, 
  0xD9, 0x5E, 0x1F, 0xF5, 0xC5, 0x33, 0x9A, 0x4A, 0x61, 0x6E, 0xBC, 0x7F, 0x16, 0x73, 0xE3, 0xB6, 
  0x14, 0x11, 0x48, 0x51, 0x47, 0x2A, 0xD6, 0xE6, 0xD2, 0x3C, 0x2A, 0x23, 0x29, 0x84, 0xA0, 0xD7, 
  0x07, 0x25, 0x1E, 0xAA, 0x0E, 0xD0, 0x54, 0xAD, 0xB4, 0x2C, 0x31, 0x44, 0x0A, 0x86, 0xD1, 0x6B, 
  0xDF, 0xC7, 0x34, 0x1C, 0x43, 0xB6, 0x5C, 0xA3, 0x50, 0xDB, 0x34, 0x6C, 0xE7, 0x5E, 0xBA, 0x8F, 
  0xA6, 0x2D, 0x5B, 0x2E, 0x8D, 0xA2, 0x73, 0x28, 0xAA, 0x11, 0x9C, 0xF9, 0xFD, 0x58, 0xE0, 0xCE, 
  0x32, 0xC4, 0xD7, 0x5B, 0x96, 0xC1, 0x49, 0xC1, 0xCD, 0x5A, 0x43, 0xC7, 0xB5, 0x4D, 0xCD, 0x0A, 
  0x65, 0x4D, 0x65, 0x7C, 0x4C, 0x0E, 0x07, 0xFE, 0xC4, 0xC3, 0xDF, 0x66, 0x40, 0xF9, 0x30, 0x31, 
  0xCD, 0x32, 0xF6, 0xAB, 0xB6, 0xA2, 0x2F, 0x89, 0x32, 0x7F, 0x15, 0x12, 0x4B, 0x68, 0xA1, 0x5C, 
  0x15, 0x66, 0x3B, 0xFC, 0x8D, 0x9E, 0x15, 0x28, 0x87, 0x1C, 0xE3, 0x8B, 0x87, 0x78, 0x53, 0x68, 
  0x8F, 0x5D, 0x54, 0x10, 0x36, 0xFA, 0x5A, 0xDA, 0x22, 0xC9, 0xEE, 0x8E, 0xAA, 0xC2, 0x4B, 0xD1, 
  0x27, 0xD4, 0x34, 0x76, 0x75, 0x90, 0x29, 0x1E, 0x99, 0x03, 0xE3, 0x62, 0x35, 0x8D, 0xBC, 0xA8, 
  0xDC, 0x7A, 0x39, 0x37, 0xD7, 0xD1, 0xDC, 0x70, 0x65, 0x86, 0x95, 0x5C, 0x2B, 0x35, 0x47, 0x48, 
  0xA5, 0x42, 0x5A, 0x62, 0x29, 0xD1, 0x47, 0xC5, 0x03, 0x31, 0xA2, 0x6E, 0x95, 0xC1, 0x6F, 0xBD, 
  0x31, 0xC0, 0x3B, 0xD4, 0xEA, 0xF1, 0xE6, 0x9A, 0x10, 0x0F, 0xFD, 0x6B, 0x22, 0xA4, 0x39, 0x28, 
  0x9A, 0xF4, 0x50, 0xD9, 0xF1, 0x3C, 0x16, 0x1E, 0x77, 0xDF, 0x9D, 0xA1, 0x21, 0x76, 0xF1, 0x96, 
  0x6A, 0x5F, 0x9B, 0x15, 0x1A, 0x26, 0x7C, 0x4C, 0xE7, 0x79, 0x38, 0xC2, 0x35, 0x1B, 0x38, 0xA2, 
  0x10, 0xB6, 0x00, 0xC7, 0xEE, 0x08, 0xA4, 0x03, 0xDB, 0x85, 0x27, 0x4C, 0xBA, 0x3F, 0x74, 0xAC, 
  0xCC, 0xEE, 0xBE, 0x27, 0xB9, 0x24, 0x45, 0x35, 0x34, 0x1B, 0x99, 0xA8, 0x5B, 0xF4, 0xB3, 0x34, 
  0x42, 0xC7, 0x0A, 0xE9, 0xF9, 0xF6, 0x2C, 0xE5, 0x6F, 0x51, 0x73, 0xD6, 0xD2, 0x9E, 0x6F, 0xAE, 
  0xA6, 0xAE, 0xED, 0xC1, 0xE9, 0x54, 0xE1, 0xB6, 0x9F, 0x94, 0x6D, 0x09, 0x98, 0x67, 0xF9, 0x85, 
  0x6C, 0xE4, 0xDF, 0x33, 0xA9, 0x47, 0x1D, 0x1B, 0x59, 0x70, 0x7E, 0xCC, 0xEA, 0x46, 0x9B, 0x17, 
  0x88, 0xE1, 0xD7, 0xB3, 0xC6, 0x15, 0x64, 0xFF, 0x07, 0xD3, 0xC6, 0xEB, 0xB5, 0x61, 0xE3, 0x81, 
  0x5C, 0xB3, 0x8A, 0xD9, 0x9F, 0x37, 0xEA, 0xF3, 0x87, 0x49, 0xED, 0xFB, 0xBC, 0x41, 0x31, 0x17, 
  0x48, 0x51, 0xFE, 0x9F, 0x9A, 0x33, 0x0B, 0x39, 0xD1, 0xD0, 0x9F, 0xB4, 0x92, 0x0D, 0x4D, 0x76, 
  0x9F, 0x83, 0x39, 0x89, 0xE9, 0x11, 0xE5, 0x01, 0x85, 0x13, 0x1B, 0x3B, 0xF7, 0x6A, 0x35, 0xD9, 
  0x7B, 0x16, 0xB5, 0xE5, 0x92, 0x08, 0x5A, 0x71, 0xD7, 0xED, 0xFA, 0x41, 0xDA, 0x2B, 0xC4, 0x01, 
  0x92, 0x99, 0x37, 0x4B, 0x67, 0xEA, 0xC9, 0x8C, 0xFE, 0xA0, 0x36, 0x4A, 0x88, 0xE6, 0xF7, 0x3A, 
  0x63, 0x7D, 0xBE, 0x64, 0x33, 0x31, 0x95, 0xBF, 0x9B, 0x98, 0x7A, 0x76, 0x3B, 0xA4, 0x4A, 0xF6, 
  0x73, 0xE5, 0x4E, 0x42, 0x89, 0x90, 0x9B, 0x1C, 0xF8, 0xF5, 0x11, 0xD0, 0x3E, 0x8F, 0x94, 0x0B, 
  0x05, 0xA4, 0x29, 0x6F, 0x63, 0xCA, 0xF8, 0x0C, 0x89, 0x7A, 0x33, 0x99, 0x2E, 0xE3, 0x22, 0x0B, 
  0x94, 0xF3, 0xED, 0xCC, 0xAA, 0x68, 0x67, 0x96, 0xD1, 0x29, 0x91, 0xC5, 0xAF, 0x35, 0xB2, 0x1A, 
  0x4C, 0x97, 0x11, 0x4A, 0x81, 0xF1, 0xEF, 0x34, 0x99, 0xF6, 0x0F, 0x88, 0x0E, 0x37, 0xCA, 0x06, 
  0xD0, 0xC4, 0x01, 0xBF, 0x15, 0x71, 0x85, 0x8F, 0x82, 0x30, 0x93, 0xE0, 0x92, 0x1D, 0x94, 0xBF, 
  0x65, 0x39, 0x76, 0xC0, 0x33, 0xB3, 0x1C, 0xB3, 0xD9, 0x57, 0x98, 0x2A, 0x83, 0x17, 0x31, 0xBF, 
  0x94, 0xB7, 0x89, 0xDB, 0x3C, 0x34, 0x7E, 0x72, 0x1C, 0xBF, 0xDF, 0x87, 0x3A, 0xF6, 0x03, 0x4E, 
  0x24, 0x74, 0xF2, 0x96, 0x2F, 0x8F, 0xF0, 0x58, 0xCC, 0x2C, 0xC1, 0x13, 0x85, 0x24, 0xF3, 0x1A, 
  0xF3, 0xBD, 0x91, 0x3F, 0x8E, 0x98, 0x3F, 0xE6, 0x4F, 0x20, 0x05, 0x22, 0x02, 0x8B, 0xBD, 0x64, 
  0x57, 0x9A, 0xE0, 0xEF, 0xBF, 0x49, 0x32, 0xB6, 0x4F, 0x4C, 0x65, 0x17, 0x71, 0x8C, 0x42, 0x7A, 
  0xF6, 0x16, 0x56, 0xA0, 0x31, 0x32, 0x43, 0xB0, 0x40, 0x1A, 0x48, 0x1E, 0xA7, 0x90, 0xDF, 0x10, 
  0xFE, 0x7B, 0x60, 0x28, 0xDB, 0xF7, 0x2D, 0x87, 0xA2, 0x05, 0xB3, 0x24, 0x78, 0xF4, 0x04, 0x2E, 
  0x49, 0x0C, 0xC1, 0x4A, 0xF1, 0x8C, 0xD1, 0xBE, 0x80, 0xD2, 0xA2, 0xF8, 0x6E, 0x46, 0x91, 0xE0, 
  0x97, 0x01, 0xE6, 0xEA, 0x21, 0x20, 0xF9, 0xB1, 0x1C, 0x03, 0x84, 0x65, 0x07, 0xDA, 0x35, 0xE1, 
  0x70, 0xA0, 0x27, 0xE4, 0x98, 0x0A, 0x5F, 0xDA, 0x63, 0xEE, 0x13, 0x8C, 0x30, 0xB5, 0xC4, 0xAC, 
  0x90, 0x36, 0x93, 0x87, 0x12, 0x5E, 0x62, 0x9B, 0x4C, 0xCD, 0x85, 0xC4, 0x69, 0xFF, 0x16, 0x14, 
  0x90, 0x4C, 0xA4, 0x62, 0x44, 0xB8, 0x2C, 0x77, 0x14, 0x68, 0x01, 0x42, 0x01, 0x14, 0x07, 0xAC, 
  0x4F, 0xC7, 0x6E, 0xDC, 0x64, 0xE0, 0x0C, 0xB6, 0x45, 0x97, 0xA1, 0x1F, 0xD0, 0x01, 0x95, 0x7D, 
  0x91, 0x9E, 0x5A, 0x00, 0xF6, 0xA2, 0x3E, 0xBD, 0x2E, 0xE4, 0x33, 0x17, 0x87, 0x38, 0xB7, 0x68, 
  0x05, 0x68, 0x5D, 0xCE, 0xC0, 0xD1, 0x41, 0xFD, 0x61, 0x9C, 0x16, 0x8A, 0xC4, 0xE1, 0x4C, 0xDE, 
  0x35, 0x48, 0x01, 0x93, 0x1B, 0x4E, 0x31, 0x51, 0x24, 0xAB, 0x9E, 0x28, 0x7F, 0xE7, 0xEE, 0x4F, 
  0x7E, 0xB8, 0x68, 0x95, 0x9B, 0x2D, 0xAB, 0x59, 0x13, 0x7E, 0x68, 0x29, 0xB1, 0xA3, 0x72, 0x4E, 
  0x39, 0x1F, 0xDF, 0x8E, 0x88, 0x17, 0x0E, 0x40, 0x52, 0xDD, 0x51, 0x1F, 0x21, 0xC2, 0x5C, 0xF5, 
  0x79, 0x6D, 0x6D, 0x31, 0x14, 0x05, 0xAB, 0xCF, 0x62, 0xFA, 0x4B, 0x59, 0xDE, 0x4D, 0x62, 0x53, 
  0x8F, 0x00, 0x66, 0x2C, 0xC6, 0xD1, 0x42, 0xB5, 0x92, 0xEB, 0xB0, 0x19, 0x9E, 0xD2, 0x7B, 0x33, 
  0x43, 0xC2, 0x95, 0xE7, 0x93, 0xB4, 0xB6, 0x82, 0x6C, 0xD5, 0x58, 0x3F, 0x64, 0xD1, 0xF0, 0x92, 
  0x0A, 0x14, 0xCB, 0x69, 0x7F, 0xE3, 0x56, 0x28, 0x1D, 0x73, 0x49, 0x7F, 0x84, 0x8F, 0xE7, 0xD0, 
  0x77, 0x47, 0x9F, 0xAB, 0xFA, 0xE2, 0x04, 0x2B, 0x94, 0x23, 0xC6, 0x63, 0x22, 0xE8, 0xDA, 0x2B, 
  0xF1, 0xD5, 0xF5, 0x8A, 0xC0, 0xB7, 0x77, 0xF2, 0x56, 0x03, 0x75, 0x29, 0x9A, 0x62, 0xC0, 0x88, 
  0xB4, 0x5B, 0xA4, 0x6A, 0x1C, 0x54, 0x0F, 0x1E, 0x2C, 0x1B, 0x90, 0x7D, 0x75, 0x0B, 0xA6, 0xFA, 
  0x42, 0x7D, 0x29, 0xA6, 0xBD, 0x53, 0x4F, 0x2B, 0x05, 0x9A, 0x06, 0x6A, 0xCB, 0x28, 0x92, 0x98, 
  0x99, 0x76, 0x4D, 0xD9, 0x5D, 0x96, 0xC1, 0x0D, 0x4D, 0xE3, 0xA0, 0x7D, 0xD6, 0xEE, 0xB6, 0xF1, 
  0x8E, 0x41, 0xF4, 0x53, 0x48, 0xAE, 0xD8, 0x14, 0xB1, 0x42, 0x6B, 0xF5, 0x54, 0x8B, 0xFA, 0x98, 
  0x96, 0x54, 0x4D, 0xC4, 0xC2, 0xCD, 0xDF, 0xD7, 0xEA, 0x2B, 0xD2, 0xCC, 0x05, 0xE9, 0x82, 0x6B, 
  0x48, 0x2A, 0xF2, 0x62, 0x8F, 0xD4, 0xAA, 0x39, 0x65, 0x18, 0x85, 0x0C, 0x00, 0x1E, 0xDC, 0xBE, 
  0xBE, 0xBE, 0xB8, 0xFE, 0x8C, 0xDD, 0x9D, 0xA2, 0x5F, 0x23, 0xC6, 0x97, 0x3A, 0xC1, 0x81, 0xDC, 
  0xFB, 0x57, 0x8D, 0x81, 0xA9, 0xBB, 0xB4, 0xD4, 0xB5, 0xCB, 0x4F, 0x9B, 0xF5, 0x69, 0xD3, 0xE6, 
  0xF8, 0xD7, 0x8F, 0xD9, 0x19, 0xD9, 0xA5, 0xC3, 0x3F, 0xAD, 0xCB, 0xB4, 0x79, 0xF0, 0xF2, 0x2B, 
  0x65, 0x9B, 0x07, 0x11, 0x21, 0x75, 0x89, 0xB3, 0x8F, 0x45, 0xE4, 0x0C, 0x86, 0xC8, 0x30, 0x5A, 
  0xB4, 0x56, 0x0E, 0xD1, 0xBF, 0xC7, 0x72, 0x7B, 0xCB, 0x2C, 0x97, 0x8B, 0x6F, 0x6F, 0x3B, 0x17, 
  0xE7, 0x78, 0xE1, 0x18, 0x31, 0x33, 0xC3, 0x7D, 0x99, 0x22, 0x97, 0x6B, 0x5D, 0x09, 0x2D, 0x5F, 
  0x2B, 0x3C, 0xE6, 0x5E, 0x86, 0xDC, 0xA8, 0x97, 0xFB, 0x49, 0xBF, 0xAF, 0x5F, 0x92, 0x17, 0x56, 
  0x1E, 0x04, 0xDC, 0x25, 0xD5, 0xB7, 0x02, 0x03, 0xD2, 0x1B, 0x73, 0xEE, 0x7B, 0x2B, 0x32, 0xD6, 
  0xD4, 0x60, 0x53, 0x8C, 0x3D, 0x01, 0xB0, 0x72, 0x91, 0x96, 0x2B, 0xB5, 0x26, 0x93, 0xC4, 0x56, 
  0xAF, 0xD5, 0x1E, 0xA8, 0x97, 0xD5, 0x9F, 0xBE, 0x4D, 0xC9, 0x30, 0x2E, 0x68, 0x38, 0x51, 0xE2, 
  0xE3, 0xD5, 0xA4, 0x2C, 0x08, 0xE5, 0x77, 0xB7, 0xA4, 0xFC, 0x38, 0x8A, 0x8A, 0x38, 0xC1, 0xB1, 
  0x27, 0xE4, 0x17, 0x6B, 0xA4, 0xF8, 0x99, 0x25, 0x0A, 0xAB, 0x15, 0x52, 0x2F, 0x4E, 0x8F, 0x20, 
  0x63, 0x3A, 0x81, 0x68, 0x42, 0xE3, 0xDB, 0xBF, 0x2C, 0x85, 0xA7, 0xEE, 0x8F, 0xF0, 0x7B, 0x4D, 
  0x39, 0x0C, 0x44, 0xBD, 0x60, 0xC8, 0x2F, 0x61, 0xA8, 0x6F, 0x7E, 0xFC, 0xFC, 0x35, 0x53, 0x86, 
  0x65, 0x92, 0xCA, 0x12, 0xE5, 0x48, 0xFE, 0x59, 0xD3, 0xCA, 0xB1, 0x9F, 0xB2, 0x6C, 0x7A, 0x49, 
  0xD6, 0xB0, 0xD2, 0xD1, 0x7E, 0xDE, 0xA4, 0x69, 0x8E, 0x62, 0x8B, 0x95, 0xE7, 0x5A, 0xEA, 0x05, 
  0x37, 0xCD, 0xBA, 0xDB, 0xB2, 0x1E, 0xFA, 0x41, 0xB7, 0x08, 0xC2, 0xFB, 0xD3, 0x99, 0xCF, 0xD4, 
  0xD9, 0xE9, 0x07, 0x36, 0x9F, 0xD7, 0x63, 0x46, 0x21, 0x4F, 0x6F, 0x8D, 0x80, 0x91, 0x35, 0x3D, 
  0x0A, 0x13, 0xE5, 0xBC, 0x0A, 0xD3, 0x08, 0xF2, 0xC4, 0x1B, 0x45, 0x9D, 0xCB, 0x60, 0xAF, 0x1C, 
  0x9E, 0x00, 0xDB, 0xE5, 0x6C, 0xED, 0x29, 0x5A, 0x68, 0x52, 0x31, 0xDF, 0xD4, 0xFF, 0x51, 0x36, 
  0x3F, 0xFF, 0xB3, 0xFC, 0x65, 0xAD, 0x50, 0x78, 0xF3, 0x1F, 0x95, 0x32, 0x9B, 0x32, 0x2B, 0xC9, 
  0xB2, 0x9F, 0xD7, 0xBF, 0x24, 0x4B, 0x14, 0xFB, 0x8A, 0x59, 0xFE, 0xA3, 0xF0, 0x8F, 0xB2, 0x58, 
  0xB3, 0x7C, 0x41, 0x7C, 0x42, 0xF5, 0x6D, 0x00, 0xB1, 0xF8, 0x05, 0x1C, 0x29, 0xFE, 0x0A, 0xC0, 
  0xDC, 0x5B, 0xA6, 0x44, 0xFE, 0x44, 0xD0, 0xC7, 0x95, 0xB9, 0x29, 0x00, 0x3A, 0x48, 0x6F, 0xF1, 
  0x00, 0x64, 0xBC, 0xB2, 0x78, 0x89, 0x33, 0x4D, 0x37, 0x4C, 0xBF, 0x52, 0x11, 0x88, 0x58, 0x2C, 
  0x2E, 0x51, 0xDD, 0x33, 0x95, 0xC2, 0xE5, 0x45, 0xA7, 0x9B, 0x5F, 0x27, 0x20, 0x52, 0x5F, 0xFA, 
  0x11, 0x87, 0x8A, 0xD9, 0x62, 0x51, 0xA4, 0x6E, 0xAA, 0xE7, 0x2B, 0x4B, 0xE9, 0x32, 0x1D, 0x11, 
  0xE9, 0xE9, 0xB0, 0x9B, 0x7F, 0x81, 0x98, 0x85, 0x04, 0xC5, 0x25, 0xBD, 0xB8, 0x7C, 0x4F, 0x5D, 
  0xF1, 0xD6, 0x5C, 0xBE, 0x44, 0xD3, 0x0E, 0x9C, 0xC9, 0xAF, 0x8B, 0xE2, 0x2C, 0x4D, 0x6D, 0x89, 
  0x97, 0xE6, 0x14, 0x24, 0xDA, 0x6E, 0xBF, 0x54, 0x8C, 0x2C, 0x79, 0xB1, 0xB4, 0x3C, 0x16, 0x55, 
  0xF6, 0xCB, 0x26, 0x35, 0xDF, 0x6B, 0x42, 0xF3, 0x77, 0x86, 0x37, 0x99, 0x05, 0xF9, 0x9D, 0xA1, 
  0x04, 0xDE, 0x04, 0xEC, 0x63, 0x82, 0x5C, 0x49, 0xC2, 0x03, 0x59, 0x83, 0x72, 0xE6, 0x5E, 0x0C, 
  0x18, 0x7A, 0xCA, 0x48, 0xA1, 0x4D, 0x06, 0x24, 0x21, 0xD2, 0x45, 0xC7, 0x14, 0x89, 0xEF, 0xC2, 
  0x91, 0xBC, 0x64, 0x6A, 0x2C, 0x7C, 0x67, 0xCE, 0x48, 0x25, 0x56, 0x10, 0x5B, 0x1E, 0x62, 0xB7, 
  0xA2, 0xBF, 0x27, 0x83, 0x9F, 0xF5, 0xBF, 0x56, 0xD8, 0xC5, 0x1E, 0x16, 0x4E, 0x23, 0x6F, 0xEB, 
  0xD3, 0xA7, 0xDA, 0xD1, 0xFF, 0x54, 0x00, 0x9A, 0x5A, 0xE2, 0xC0, 0xA4, 0x7A, 0x87, 0x4C, 0xC4, 
  0xED, 0x4C, 0xFC, 0xB8, 0xBF, 0x5B, 0x01, 0x82, 0x39, 0xD2, 0x45, 0x89, 0x72, 0xC9, 0xE2, 0xD3, 
  0xE7, 0xCE, 0x26, 0x5F, 0xF9, 0x52, 0x92, 0xC0, 0x5C, 0xB0, 0xDF, 0x11, 0xA3, 0xE4, 0x04, 0x86, 
  0xEB, 0xBB, 0x95, 0x20, 0x9E, 0xE9, 0x85, 0xF1, 0x47, 0x47, 0xBC, 0x93, 0x98, 0xE3, 0x21, 0xBE, 
  0x36, 0x66, 0x90, 0x28, 0xB4, 0xF6, 0xD4, 0x4B, 0x8A, 0x34, 0xFF, 0xDD, 0x8A, 0x5C, 0xA4, 0x44, 
  0x48, 0x49, 0x93, 0x62, 0x96, 0x7D, 0xE7, 0x01, 0xBC, 0xF0, 0xAE, 0x68, 0x6F, 0x35, 0xF3, 0xED, 
  0xA7, 0xD5, 0x0C, 0xAB, 0xDD, 0x0A, 0xAA, 0x77, 0x7F, 0x05, 0xF4, 0x2D, 0xFE, 0xDD, 0xC8, 0xFF, 
  0x02, 0x63, 0xB6, 0x52, 0xB6, 0xAA, 0x32, 0x00, 0x00
};


//===============================================================
// Constructor
//===============================================================
SPIFFSEditor::SPIFFSEditor() :
  _startTime(0)
{
}

//===============================================================
// Returns true, if the handler can handle the request
//===============================================================
bool SPIFFSEditor::canHandle(AsyncWebServerRequest* request) const
{
  if (request->url().equalsIgnoreCase("/edit"))
  {
    if (request->method() == HTTP_GET)
    {
      if (request->hasParam("list") ||
        request->hasParam("systeminfo") ||
        request->hasParam("format"))
      {
        return true;
      }
      else if (request->hasParam("edit"))
      {
        request->_tempFile = SPIFFS.open(request->arg("edit"), "r");
        if (!request->_tempFile)
        {
          return false;
        }

        if (request->_tempFile.isDirectory())
        {
          request->_tempFile.close();
          return false;
        }
      }
      else if (request->hasParam("download"))
      {
        request->_tempFile = SPIFFS.open(request->arg("download"), "r");
        if (!request->_tempFile)
        {
          return false;
        }

        if (request->_tempFile.isDirectory())
        {
          request->_tempFile.close();
          return false;
        }
      }
      request->addInterestingHeader("If-Modified-Since");
      return true;
    }
    else if (request->method() == HTTP_POST)
    {
      return true;
    }
    else if(request->method() == HTTP_DELETE)
    {
      return true;
    }
  }
  return false;
}

//===============================================================
// Handles the request
//===============================================================
void SPIFFSEditor::handleRequest(AsyncWebServerRequest* request)
{
  if (request->method() == HTTP_GET)
  {
    if (request->hasParam("format"))
    {
      SPIFFS.format();
      request->send(200, "", "FORMAT: SPIFFS sucessfully formatted");
    }
    else if (request->hasParam("systeminfo"))
    {
      request->send(200, "", GetSystemInfoString());
    }
    else if (request->hasParam("list"))
    {
      String path = request->getParam("list")->value();
      
      // Open directory
      File dir = SPIFFS.open(path);
      
      path = String();
      String output = "[";

      // Open first file
      File entry = dir.openNextFile();

      // Iterate through all files
      while (entry)
      {
        if (output != "[")
        {
          output += ',';
        }
        output += "{\"type\":\"";
        output += "file";
        output += "\",\"name\":\"";
        output += String(entry.name());
        output += "\",\"size\":";
        output += String(entry.size());
        output += "}";

        entry = dir.openNextFile();
      }

      dir.close();

      output += "]";
      request->send(200, "application/json", output);
      output = String();
    }
    else if (request->hasParam("edit") ||
      request->hasParam("download"))
    {
      request->send(request->_tempFile, request->_tempFile.name(), String(), request->hasParam("download"));
    }
    else
    {
      const char * buildTime = __DATE__ " " __TIME__ " GMT";
      if (request->header("If-Modified-Since").equals(buildTime))
      {
        request->send(304);
      }
      else
      {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", edit_html_gz, edit_html_gz_len);
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Last-Modified", buildTime);
        request->send(response);
      }
    }
  }
  else if (request->method() == HTTP_DELETE)
  {
    // Check for path param
    if (!request->hasParam("path", true))
    {
      request->send(404, "", "DELETE: Error deleting file, path parameter missing");
      return;
    }

    if (!SPIFFS.remove(request->getParam("path", true)->value()))
    {
      request->send(404, "", "DELETE: Error deleting file, SPIFFS error");
      return;
    }

    request->send(200, "", "DELETE: " + request->getParam("path", true)->value());
  }
  else if (request->method() == HTTP_POST)
  {
    if (request->hasParam("data", true, true) &&
      SPIFFS.exists(request->getParam("data", true, true)->value()))
    {
      request->send(200, "", "UPLOAD: " + request->getParam("data", true, true)->value());
    }
    else
    {
      request->send(500, "", "UPLOAD: Error uploading file");
    }
  }
}

//===============================================================
// Handles the upload
//===============================================================
void SPIFFSEditor::handleUpload(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final)
{
  if (!index)
  {
    request->_tempFile = SPIFFS.open(filename, "w");
    _startTime = millis();
  }
  if (request->_tempFile)
  {
    if (len)
    {
      request->_tempFile.write(data,len);
    }
    if (final)
    {
      request->_tempFile.close();
    }
  }
}
