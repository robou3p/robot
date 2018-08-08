#include "IMU.h"

IMU::IMU()
{
  //  mpu.begin();
}

float IMU::getRoll()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    filters.MadgwickQuaternionUpdate(mpu._ax, mpu._ay, mpu._az, mpu._gx, mpu._gy, mpu._gz, mpu._hx, mpu._hy, mpu._hz);
    lastUpdate = micros();
  }
  return filters.roll;
}

float IMU::getPitch()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    filters.MadgwickQuaternionUpdate(mpu._ax, mpu._ay, mpu._az, mpu._gx, mpu._gy, mpu._gz, mpu._hx, mpu._hy, mpu._hz);
    lastUpdate = micros();
  }
  return filters.pitch;
}

float IMU::getYaw()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    filters.MadgwickQuaternionUpdate(mpu._ax, mpu._ay, mpu._az, mpu._gx, mpu._gy, mpu._gz, mpu._hx, mpu._hy, mpu._hz);
    lastUpdate = micros();
  }
  return filters.yaw;
}

float IMU::getAX()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._ax;
}

float IMU::getAY()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._ay;
}

float IMU::getAZ()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._az;
}

float IMU::getGX()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._gx;
}

float IMU::getGY()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._gy;
}

float IMU::getGZ()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._gz;
}

float IMU::getMX()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._hx;
}

float IMU::getMY()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._hy;
}

float IMU::getMZ()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  if (micros() - lastUpdate > 1000)
  {
    mpu.readSensor();
    lastUpdate = micros();
  }
  return mpu._hz;
}

void IMU::calibrate()
{
  if (lastUpdate == 0)
  {
    mpu.begin();
  }
  mpu.calibrateMag();
}