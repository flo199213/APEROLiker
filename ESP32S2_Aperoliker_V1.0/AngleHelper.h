/**
 * Includes all angle functions
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
 */
 
#ifndef ANGLEHELPER_H
#define ANGLEHELPER_H

//===============================================================
// Includes
//===============================================================
#include "Config.h"
#include <Arduino.h>

//===============================================================
// Declarations
//===============================================================

// Moves a value in 360 degrees space around the specified positive or negative distance
int16_t Move360(int16_t value, int16_t distance);

// Return the clockwise distance between two angles in an 360° space
int16_t GetDistanceDegrees(int16_t startAngle, int16_t stopAngle);

#endif
