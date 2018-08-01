#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Pinout.h"

class Button
{
public:
  Button();
  uint8_t pressed();
  void wait();
  void press();

private:
  uint8_t hasBeenPressed = 0;
};

#endif