#ifndef LINE_H
#define LINE_H

#define BLACK 0
#define WHITE 1

#include "Arduino.h"
#include "Pinout.h"
#include "EEPROM.h"
#include <math.h>

class Line
{
public:
  Line();
  void calibrate(int16_t duration);
  float getPosition();
  float getAngle();
  float setNoiseThreshold(int16_t threshold);
  float setColor(uint8_t color);
  uint16_t getSensor(uint8_t sensor);
  float getLength();
  void setLength(float length);

  //private:
  void readCalibrated();
  void readRaw();
  int16_t sensorsRaw[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int16_t sensorsMin[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
  int16_t sensorsMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int16_t sensorsCalibrated[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int16_t linePrevious = 0;
  int16_t noiseThreshold = 500;
  uint8_t color = BLACK;
  float length = 0.082;
  uint32_t lastUpdate = 0;
};

#endif
