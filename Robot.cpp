#include "Robot.h"

/*
 * Constructor
 */
Robot::Robot()
{
  // motors
  pinMode(PWMRIGHT, OUTPUT);
  digitalWrite(PWMRIGHT, LOW);
  pinMode(PWMLEFT, OUTPUT);
  digitalWrite(PWMLEFT, LOW);
  pinMode(DIRRIGHT1, OUTPUT);
  digitalWrite(DIRRIGHT1, LOW);
  pinMode(DIRRIGHT2, OUTPUT);
  digitalWrite(DIRRIGHT2, LOW);
  pinMode(DIRLEFT1, OUTPUT);
  digitalWrite(DIRLEFT1, LOW);
  pinMode(DIRLEFT2, OUTPUT);
  digitalWrite(DIRLEFT2, LOW);
  // encoders
  pinMode(ENCRIGHTINT, INPUT_PULLUP);
  pinMode(ENCRIGHTDIR, INPUT);
  pinMode(ENCLEFTINT, INPUT_PULLUP);
  pinMode(ENCLEFTDIR, INPUT);
  // interfaces
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(BUTTON, INPUT);
  // sensors
  pinMode(MUXL0, OUTPUT);
  digitalWrite(MUXL0, LOW);
  pinMode(MUXL1, OUTPUT);
  digitalWrite(MUXL1, LOW);
  pinMode(MUXL2, OUTPUT);
  digitalWrite(MUXL2, LOW);
  pinMode(DISTLED, OUTPUT);
  digitalWrite(DISTLED, HIGH);
  pinMode(LINELED, OUTPUT);
  digitalWrite(LINELED, HIGH);
  pinMode(DISTSEN, INPUT);
  pinMode(LINESEN, INPUT);
  // setup encoder input capture interrupts
  setupEncoders();
  // setup serial communication
  Serial.begin(115200);
  Serial1.begin(115200);
}

/*
 * Stops the robot.
 */
void Robot::stop()
{
  motor[LEFT].setVoltage(0);
  motor[RIGHT].setVoltage(0);
}

/*
 * Drive with v (m/s) and w(rad/s).
 */
void Robot::drive(float v, float w_degs)
{
  float w = w_degs * M_PI / 180.0;
  motor[LEFT].setSpeed(v * 2 / motor[LEFT].getDiameter() + w * width / motor[LEFT].getDiameter());
  motor[RIGHT].setSpeed(v * 2 / motor[RIGHT].getDiameter() - w * width / motor[RIGHT].getDiameter());
}

/*
 * Go straight for a defined distance.
 */
void Robot::go(float distance)
{
  go(distance, 0.3f);
}

void Robot::go(float distance, float v)
{
  float current_distance = (motor[LEFT].getDistance() + motor[RIGHT].getDistance()) / 2.0f;
  if (distance >= 0)
  {
    while ((motor[LEFT].getDistance() + motor[RIGHT].getDistance()) / 2.0f <= current_distance + distance)
    {
      drive(fabs(v), 0.0f);
    }
  }
  else
  {
    while ((motor[LEFT].getDistance() + motor[RIGHT].getDistance()) / 2.0f >= current_distance + distance)
    {
      drive(-fabs(v), 0.0f);
    }
  }
  stop();
}

/*
 * Turn for a defined angle.
 */
void Robot::turn(float angle_deg)
{
  turn(angle_deg, 180.0f);
}

void Robot::turn(float angle_deg, float w_degs)
{
  float angle = angle_deg;
  float w = w_degs;
  float current_angle = (motor[LEFT].getDistance() - motor[RIGHT].getDistance()) / width;
  if (angle >= 0)
  {
    while ((motor[LEFT].getDistance() - motor[RIGHT].getDistance()) / width <= current_angle + angle * M_PI / 180.0)
    {
      drive(0, fabs(w));
    }
  }
  else
  {
    while ((motor[LEFT].getDistance() - motor[RIGHT].getDistance()) / width >= current_angle + angle * M_PI / 180.0)
    {
      drive(0, -fabs(w));
    }
  }
  stop();
}

/*
 * Beeps with the specified frequency and duration.
 */
void Robot::beep(int16_t frequency, int16_t duration)
{
  int32_t tStart = micros();
  int16_t t = (int16_t)(500000L / (int32_t)frequency);
  while (micros() < tStart + (int32_t)duration * 10 * NOTE_PERCENT)
  {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(t);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(t);
  }
  delayMicroseconds((int32_t)duration * 10 * (100 - NOTE_PERCENT));
}

/*
 * Sets the distance between the wheels.
 */
void Robot::setWidth(float width)
{
  this->width = width;
}

/*
 * Returns the current robot speed in m/s.
 */
float Robot::getSpeed()
{
  return (M_PI * motor[LEFT].getDiameter() * motor[LEFT].getSpeed() + M_PI * motor[RIGHT].getDiameter() * motor[RIGHT].getSpeed()) / 2.0;
}

/*
 * Returns the current robot angular velocity in deg/s.
 */
float Robot::getAngularVelocity()
{
  return (180.0f * motor[LEFT].getDiameter() * motor[LEFT].getSpeed() - 180.0f * motor[RIGHT].getDiameter() * motor[RIGHT].getSpeed()) / width;
}

/*
 * Sets up the TIMER1 and TIMER3 interrupts.
 */
void Robot::setupEncoders()
{
  // Input Capture setup
  // ICNC1: =0 Disable Input Capture Noise Canceler to prevent delay in reading
  // ICES1: =1 for trigger on rising edge
  // CS11: =1 set prescaler to 1/8 system clock (F_CPU)
  TCCR1A = 0;
  TCCR1B = (0 << ICNC1) | (1 << ICES1) | (1 << CS11);
  TCCR1C = 0;
  // Interrupt setup
  // ICIE1: Input capture, TOIE1: Overflow
  TIFR1 = (1 << ICF1);                  // clear pending
  TIMSK1 = (1 << ICIE1) | (1 << TOIE1); // and enable
  // Timer 3
  TCCR3A = 0;
  TCCR3B = (0 << ICNC3) | (1 << ICES3) | (1 << CS31);
  TCCR3C = 0;
  TIFR3 = (1 << ICF3);                  // clear pending
  TIMSK3 = (1 << ICIE3) | (1 << TOIE3); // and enable
}

Robot robot = Robot();

/*
 * Capture and overflow interrupts - call respective ISRs.
 */
ISR(TIMER1_CAPT_vect)
{
  robot.motor[RIGHT].CAPT_ISR(RIGHT);
}

ISR(TIMER3_CAPT_vect)
{
  robot.motor[LEFT].CAPT_ISR(LEFT);
}

ISR(TIMER1_OVF_vect)
{
  robot.motor[RIGHT].OVF_ISR();
}

ISR(TIMER3_OVF_vect)
{
  robot.motor[LEFT].OVF_ISR();
}
