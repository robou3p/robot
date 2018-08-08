#include "Distance.h"

/*
 * Constructor.
 */
Distance::Distance()
{
  // loads default calibration values

  for (uint16_t i = 0; i < 6; i++)
  {
    EEPROM.get(16 * sizeof(int16_t) + (i) * sizeof(float), a[i]);
    EEPROM.get(16 * sizeof(int16_t) + (i + 6) * sizeof(float), b[i]);
  }
}

/*
 * Calibrates the sensors: the (prototype) procedure is to show min and max values for each sensor.
 * An inverse logarithmis curve is fitted.
 */
void Distance::calibrate(int16_t duration)
{
  uint32_t tStart = millis();
  do
  {
    readRaw();
    for (uint8_t i = 0; i < 6; i++)
    {
      if (sensorsRaw[i] > sensorsMax[i])
        sensorsMax[i] = sensorsRaw[i];
      if ((sensorsRaw[i] < sensorsMin[i]))
        sensorsMin[i] = sensorsRaw[i];
    }
  } while (millis() < tStart + duration);
  for (uint16_t i = 0; i < 6; i++)
  {
    a[i] = 0.5 * log((float)sensorsMax[i]) * log((float)sensorsMin[i]) / (log((float)sensorsMax[i]) - log((float)sensorsMin[i]));
    b[i] = 0.5 * log((float)sensorsMin[i]) / (log((float)sensorsMax[i]) - log((float)sensorsMin[i]));
    EEPROM.put(16 * sizeof(int16_t) + (i) * sizeof(float), a[i]);
    EEPROM.put(16 * sizeof(int16_t) + (i + 6) * sizeof(float), b[i]);
  }
}

/*
 * Reads the calibrated value of a single sensor.
 */
float Distance::getDistance(uint8_t sensor)
{
  readCalibrated();
  return sensorsCalibrated[sensor];
}

/*
 * Reads raw distance sensor data.
 */
void Distance::readRaw()
{
  int16_t onValues[6] = {1023, 1023, 1023, 1023, 1023, 1023};
  digitalWrite(DISTLED, LOW);
  for (uint8_t i = 1; i < 7; i++)
  {
    digitalWrite(MUXL0, bitRead(i, 0));
    digitalWrite(MUXL1, bitRead(i, 1));
    digitalWrite(MUXL2, bitRead(i, 2));
    delayMicroseconds(5);
    onValues[i - 1] = analogRead(DISTSEN);
  }
  digitalWrite(DISTLED, HIGH);
  for (uint8_t i = 1; i < 7; i++)
  {
    digitalWrite(MUXL0, bitRead(i, 0));
    digitalWrite(MUXL1, bitRead(i, 1));
    digitalWrite(MUXL2, bitRead(i, 2));
    delayMicroseconds(5);
    sensorsRaw[i - 1] = onValues[i - 1] - analogRead(DISTSEN);
  }
  lastUpdate = micros();
}

/*
 * Reads raw distance sensor data.
 */
void Distance::readRaw(uint8_t i)
{
  int16_t onValues[6] = {1023, 1023, 1023, 1023, 1023, 1023};
  digitalWrite(DISTLED, LOW);
  digitalWrite(MUXL0, bitRead(i, 0));
  digitalWrite(MUXL1, bitRead(i, 1));
  digitalWrite(MUXL2, bitRead(i, 2));
  delayMicroseconds(5);
  onValues[i] = analogRead(DISTSEN);
  digitalWrite(DISTLED, HIGH);
  digitalWrite(MUXL0, bitRead(i, 0));
  digitalWrite(MUXL1, bitRead(i, 1));
  digitalWrite(MUXL2, bitRead(i, 2));
  delayMicroseconds(5);
  sensorsRaw[i] = onValues[i - 1] - analogRead(DISTSEN);
}

/*
 * Reads calibrated values.
 */
void Distance::readCalibrated()
{
  readRaw();
  for (uint8_t i = 0; i < 6; i++)
  {
    sensorsCalibrated[i] = a[i] / log(sensorsRaw[i]) - b[i];
    if (sensorsCalibrated[i] < 0.0)
      sensorsCalibrated[i] = 0.0;
    else if (sensorsCalibrated[i] > 1.0)
      sensorsCalibrated[i] = 1.0;
  }
}

/*
 * Reads calibrated values.
 */
void Distance::readCalibrated(uint8_t i)
{
  if (micros() - lastUpdate > 1000)
  {
    readRaw(i);
  }
  sensorsCalibrated[i] = a[i] / log(sensorsRaw[i]) - b[i];
  if (sensorsCalibrated[i] < 0.0)
    sensorsCalibrated[i] = 0.0;
  else if (sensorsCalibrated[i] > 1.0)
    sensorsCalibrated[i] = 1.0;
}
