#include "Button.h"

/**
 * @brief Construct a new Button:: Button object
 * 
 */
Button::Button()
{
}

/**
 * @brief Checks whether the button is pressed.
 * 
 * @return uint8_t Button state, true if pressed.
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

/**
 * @brief Waits for the button to be pressed.
 * 
 */
void Button::wait()
{
  while (!pressed())
  {
    delay(10);
  }
}

/**
 * @brief Simulates a button press.
 * 
 */
void Button::press()
{
  hasBeenPressed = 1;
}