#include "Battery.h"

/*
 * Constructor
 */
Battery::Battery()
{
}

/*
 * Returns the battery voltage
 */
float Battery::getVoltage()
{
  digitalWrite(MUXL0, LOW);
  digitalWrite(MUXL1, LOW);
  digitalWrite(MUXL2, LOW);
  return analogRead(BATSEN) * 4.97 / 1024.0;
}
