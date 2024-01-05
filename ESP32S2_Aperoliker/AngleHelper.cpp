/**
 * Includes all angle functions
 *
 * @author    Florian Staeblein
 * @date      2023/09/15
 * @copyright © 2023 Florian Staeblein
 */

#include "AngleHelper.h"

//===============================================================
// Moves a value in 360 degrees space around the specified distance
//===============================================================
int16_t Move360(int16_t value, int16_t distance)
{
  int newValue = value + distance;
  if (newValue >= 360)
  {
    newValue -= 360;
  }
  if (newValue < 0)
  {
    newValue += 360;
  }
  return newValue;
}

//===============================================================
// Return the clockwise distance between two angles in an 360°
// space
//===============================================================
int16_t GetDistanceDegrees(int16_t startAngle, int16_t stopAngle)
{
  // Set inital variables
  int16_t distance = 0;
  int16_t runAngle = startAngle;
  
  // Calculate distance between two angles clock wise with regard to an overflow over 0/360°
  while(runAngle != stopAngle)
  {
    runAngle = Move360(runAngle, 1);
    distance++;
  }
 
  return distance;
}
