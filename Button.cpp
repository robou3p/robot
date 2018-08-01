#include "Button.h"

/*
 * Constructor
 */
Button::Button()
{
}

/*
 * Returns the state of the button.
 */
uint8_t Button::pressed()
{
  if (hasBeenPressed)
  {
    hasBeenPressed = 0;
    return 1;
  }
  return !digitalRead(BUTTON);
}

/*
 * Waits for the button to be pressed.
 */
void Button::wait()
{
  while (!pressed())
  {
    delay(10);
  }
}

/*
 * Simulates a button press.
 */
void Button::press()
{
  hasBeenPressed = 1;
}