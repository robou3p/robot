#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "Pinout.h"

#define TIMER1 1
#define TIMER3 3

#define LEFT 0
#define RIGHT 1

class Motor
{
public:
  Motor(uint8_t motor);
  float getVoltage();
  void setVoltage(float u);
  float getSpeed();
  void setSpeed(float w);
  void setSpeedGains(float kp, float ki, float kd);
  void setSpeedILimit(float limit);
  void resetI();
  float getDistance();
  void resetDistance();
  float getDistanceRad();
  float getDiameter();
  void setDiameter(float diameter);
  void CAPT_ISR(uint8_t motor);
  void OVF_ISR();

private:
  uint8_t motor = 0;
  float voltage = 0.0;
  volatile float speed = 0.0;
  float distance = 1.0;
  float diameter = 0.030;
  float kp = 0.15;
  float ki = 0.6;
  float kd = 0.0;
  volatile uint32_t encoderDt = 1000000000;
  volatile int8_t encoderDirection = 1;
  volatile int32_t encoderTicks = 0;
  volatile uint32_t encoderLast[4] = {0, 0, 0, 0};
  volatile uint32_t encoderOverflows = 0;
  volatile uint32_t motorLast = 0;
  float errorIntegral = 0.0;
  float errorIntegralLimit = 2.0;
  float errorPrevious = 0.0;
};

#endif
