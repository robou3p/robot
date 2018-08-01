#ifndef IMU_H
#define IMU_H

#include "MPU9250.h"
#include "Filters.h"

class IMU
{
public:
  IMU();
  float getRoll();
  float getPitch();
  float getYaw();
  float getAX();
  float getAY();
  float getAZ();
  float getGX();
  float getGY();
  float getGZ();
  float getMX();
  float getMY();
  float getMZ();

  void calibrateMag();

  Filters filters = Filters();
  MPU9250 mpu = MPU9250(Wire, 0x68);

private:
  uint32_t lastUpdate = 0;
};

#endif