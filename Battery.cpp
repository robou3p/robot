#include "Battery.h"

/**
 * @brief Construct a new Battery:: Battery object
 * 
 */
Battery::Battery()
{
}

/**
 * @brief Returns the current battery voltage.
 * 
 * @return float Battery voltage.
 */
float Battery::getVoltage()
{
  digitalWrite(MUXL0, LOW);
  digitalWrite(MUXL1, LOW);
  digitalWrite(MUXL2, LOW);
  return analogRead(BATSEN) * 4.97 / 1024.0;
}
