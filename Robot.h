#ifndef ROBOT_H
#define ROBOT_H

#include "Arduino.h"
#include "Pinout.h"
#include "Button.h"
#include "Battery.h"
#include "Motor.h"
#include "Line.h"
#include "Distance.h"
#include "IMU.h"
#include "Notes.h"
#include "Bluetooth.h"
#include "ROSInterface.h"
#include <avr/interrupt.h>
#include <math.h>

#define STOP 1
#define NOSTOP 0

class Robot
{
public:
  Robot();
  void stop();
  void go(float distance);
  void go(float distance, float v);
  void turn(float angle);
  void turn(float angle, float w);
  void drive(float v, float w);
  void beep(int16_t frequency, int16_t duration);
  void setWidth(float width);
  float getSpeed();
  float getAngularVelocity();

  Button button = Button();
  Battery battery = Battery();
  Motor motor[2] = {Motor(LEFT), Motor(RIGHT)};
  Line line = Line();
  Distance distance = Distance();
  IMU imu = IMU();
  Bluetooth bluetooth = Bluetooth();
  ROSInterface ros = ROSInterface();

private:
  void setupEncoders();
  float width = 0.084;
};

extern Robot robot;

#endif
