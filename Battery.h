#ifndef BATTERY_H
#define BATTERY_H

#include "Arduino.h"
#include "Pinout.h"

class Battery
{
public:
  Battery();
  float getVoltage();

private:
};

#endif