#include "Motor.h"

/*
 * Constructor - define which pins the motor uses
 */
Motor::Motor(uint8_t motor)
{
  this->motor = motor;
}

/*
 * Returns the voltage on the motor.
 */
float Motor::getVoltage()
{
  return voltage;
}

/*
 * Sets the voltage on the motor.
 */
void Motor::setVoltage(float u)
{
  voltage = constrain(u, -5.0, 5.0);
  int16_t pwm = (int16_t)(voltage * 255.0 / 8.82);
  pwm = constrain(pwm, -255, 255);
  if (motor == RIGHT)
  {
    digitalWrite(DIRRIGHT1, pwm >= 0);
    digitalWrite(DIRRIGHT2, pwm < 0);
    analogWrite(PWMRIGHT, (uint8_t)abs(pwm));
  }
  else
  {
    digitalWrite(DIRLEFT2, pwm >= 0);
    digitalWrite(DIRLEFT1, pwm < 0);
    analogWrite(PWMLEFT, (uint8_t)abs(pwm));
  }
}

/*
 * Gets the omega of the wheel in rad/s
 */
float Motor::getSpeed()
{
  return encoderDirection * 8000000.0 * M_PI / 6.0 / 29.86 / (float)encoderDt;
}

/*
 * PID speed control, input in rad/s
 */
void Motor::setSpeed(float w)
{
  uint32_t now = micros();
  uint32_t motorDt = now - motorLast;
  motorLast = now;
  float error = w - getSpeed();
  errorIntegral = errorIntegral + error * motorDt / 1000000.0;
  if (errorIntegral > errorIntegralLimit)
    errorIntegral = errorIntegralLimit;
  else if (errorIntegral < -errorIntegralLimit)
    errorIntegral = -errorIntegralLimit;
  int16_t errorDifferential = (error - errorPrevious) * 1000000.0 / motorDt;
  setVoltage(kp * error + ki * errorIntegral + kd * errorDifferential);
  errorPrevious = error;
}

/*
 * Set gains for speed PID control;
 */
void Motor::setSpeedGains(float kp, float ki, float kd)
{
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
}

/*
 * Sets the PID integral limit.
 */
void Motor::setSpeedILimit(float limit)
{
  errorIntegralLimit = limit;
}

/*
 * Return distance in m.
 */
float Motor::getDistance()
{
  return encoderTicks * diameter * M_PI / 6.0 / 29.86;
}

/*
 * Reset distance to 0.
 */
void Motor::resetDistance()
{
  encoderTicks = 0;
}

/*
 * Return distance in rads.
 */
float Motor::getDistanceRad()
{
  return encoderTicks * 2 * M_PI / 6.0 / 29.86;
}

/*
 * Get the wheel diameter in m.
 */
float Motor::getDiameter()
{
  return diameter;
}

/*
 * Set the wheel diameter in m.
 */
void Motor::setDiameter(float diameter)
{
  this->diameter = diameter;
}

/*
 * Timer 1 and 3 input capture interrupt service routine
 */
void Motor::CAPT_ISR(uint8_t motor)
{
  uint32_t now = micros();
  encoderDt = now - encoderLast[3];
  encoderLast[3] = encoderLast[2];
  encoderLast[2] = encoderLast[1];
  encoderLast[1] = encoderLast[0];
  encoderLast[0] = now;
  if (motor == RIGHT)
  {
    encoderDirection = digitalRead(ENCRIGHTDIR) ? 1 : -1;
    // detect both rising and falling
    TCCR1B &= ~(1 << ICES1);
  }
  else if (motor == LEFT)
  {
    encoderDirection = digitalRead(ENCLEFTDIR) ? -1 : 1;
    // detect both rising and falling
    TCCR3B &= ~(1 << ICES3);
  }
  encoderTicks += encoderDirection;
  encoderOverflows = 0;
}

/*
 * Timer 1 and 3 overflow interrupt service routine
 */
void Motor::OVF_ISR()
{
  if (++encoderOverflows > 64)
  {
    speed = 0.0;
  }
}
