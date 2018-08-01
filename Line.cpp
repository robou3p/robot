#include "Line.h"

/*
 * Constructor
 */
Line::Line()
{
  // loads default calibration values
  for (uint16_t i = 0; i < 8; i++)
  {
    EEPROM.get(i * sizeof(int16_t), sensorsMin[i]);
    EEPROM.get((i + 8) * sizeof(int16_t), sensorsMax[i]);
  }
}

/*
 * Line calibration, duration in ms.
 */
void Line::calibrate(int16_t duration)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    sensorsMin[i] = 1023;
    sensorsMax[i] = 0;
  }
  uint32_t tStart = millis();
  do
  {
    readRaw();
    for (uint8_t i = 0; i < 8; i++)
    {
      if (sensorsRaw[i] > sensorsMax[i])
        sensorsMax[i] = sensorsRaw[i];
      if ((sensorsRaw[i] < sensorsMin[i]))
        sensorsMin[i] = sensorsRaw[i];
    }
  } while (millis() < tStart + duration);
  for (uint16_t i = 0; i < 8; i++)
  {
    EEPROM.put(i * sizeof(int16_t), sensorsMin[i]);
    EEPROM.put((i + 8) * sizeof(int16_t), sensorsMax[i]);
  }
}

/*
 * Weighted average in m from centre.
 */
float Line::getPosition()
{
  readCalibrated();
  int32_t weightedSum = 0;
  int16_t sum = 0;
  uint8_t onLine = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    int value = sensorsCalibrated[i];
    if (color == WHITE)
    {
      value = 1023 - value;
    }
    if (value > noiseThreshold)
    {
      onLine += (1 << i);
      weightedSum += (int32_t)value * i * 1000;
      sum += value;
    }
  }
  if (onLine)
  {
    if (onLine & 1)
    {
      linePrevious = -3500;
    }
    else if (onLine & (1 << 8))
    {
      linePrevious = 3500;
    }
    else
    {
      linePrevious = (int16_t)(weightedSum / sum - 3500);
    }
  }
  else
  {
    if (linePrevious < 0)
      linePrevious = -3500;
    else
      linePrevious = 3500;
  }

  return linePrevious / 100000.0;
}

/*
 * Returns the angle of the line with respect to the robot's centre of rotation.
 */
float Line::getAngle()
{
  return atan2(getPosition(), length);
}

/*
 * Sets the threshold for individual sensor filtering.
 */
float Line::setNoiseThreshold(int16_t threshold)
{
  noiseThreshold = threshold;
}

/*
 * Sets the color of the line (BLACK or WHITE).
 */
float Line::setColor(uint8_t color)
{
  this->color = color;
}

/*
 * Gets the calibrated value of an idividual sensor.
 */
uint16_t Line::getSensor(uint8_t sensor)
{
  if (micros() - lastUpdate > 1000)
  {
    readCalibrated();
  }
  return sensorsCalibrated[sensor];
}

/*
 * Gets the distance from the centre of rotation to the sensors.
 */
float Line::getLength()
{
  return length;
}

/*
 * Sets the distance from the centre of rotation to the sensors.
 */
void Line::setLength(float length)
{
  this->length = length;
}

/*
 * Reads raw sensor values into sensorsRaw array.
 */
void Line::readRaw()
{
  int16_t sensorsOn[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
  digitalWrite(LINELED, LOW);
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(MUXL0, bitRead(i, 0));
    digitalWrite(MUXL1, bitRead(i, 1));
    digitalWrite(MUXL2, bitRead(i, 2));
    delayMicroseconds(300);
    sensorsOn[i] = analogRead(LINESEN);
  }
  digitalWrite(LINELED, HIGH);
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(MUXL0, bitRead(i, 0));
    digitalWrite(MUXL1, bitRead(i, 1));
    digitalWrite(MUXL2, bitRead(i, 2));
    delayMicroseconds(300);
    sensorsRaw[i] = sensorsOn[i] + 1023 - analogRead(LINESEN);
  }
  lastUpdate = micros();
}

/*
 * Reads calibrated values (between 0 and 1000).
 */
void Line::readCalibrated()
{
  readRaw();
  for (uint8_t i = 0; i < 8; i++)
  {
    sensorsCalibrated[i] = (int16_t)((((int32_t)(sensorsRaw[i] - sensorsMin[i])) * 1000) / (sensorsMax[i] - sensorsMin[i]));
    if (sensorsCalibrated[i] < 0)
      sensorsCalibrated[i] = 0;
    else if (sensorsCalibrated[i] > 1000)
      sensorsCalibrated[i] = 1000;
  }
}
