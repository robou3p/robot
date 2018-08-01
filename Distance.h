#ifndef DISTANCE_H
#define DISTANCE_H

#include "Arduino.h"
#include "Pinout.h"
#include "EEPROM.h"

class Distance
{
public:
  Distance();
  void calibrate(int16_t duration);
  float getDistance(uint8_t sensor);

  // private:
  void readCalibrated();
  void readCalibrated(uint8_t i);
  void readRaw();
  void readRaw(uint8_t i);
  int16_t sensorsRaw[6] = {0, 0, 0, 0, 0, 0};
  int16_t sensorsMin[6] = {1023, 1023, 1023, 1023, 1023, 1023};
  int16_t sensorsMax[6] = {0, 0, 0, 0, 0, 0};
  float a[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  float b[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  float sensorsCalibrated[6] = {0, 0, 0, 0, 0, 0};
  uint32_t lastUpdate = 0;
};

#endif
