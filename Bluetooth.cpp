#include "Bluetooth.h"
#include "Robot.h"

void Bluetooth::rename()
{
    if (Serial.available())
    {
        Serial1.write(Serial.read());
    }
    if (Serial1.available())
    {
        Serial.write(Serial1.read());
    }
}

void Bluetooth::control()
{
    if (Serial1.available())
    {
        switch (Serial1.read())
        {
        case ('L'):
        {
            while (!Serial1.available())
            {
            }
            int dirLeft = Serial1.read();
            while (!Serial1.available())
            {
            }
            int speedLeft = Serial1.read();
            if (dirLeft == 1)
            {
                robot.motor[LEFT].setVoltage(speedLeft / 51.0);
            }
            else
            {
                robot.motor[LEFT].setVoltage(-speedLeft / 51.0);
            }
            break;
        }
        case ('R'):
        {
            while (!Serial1.available())
            {
            }
            int dirRight = Serial1.read();
            while (!Serial1.available())
            {
            }
            int speedRight = Serial1.read();
            if (dirRight == 1)
            {
                robot.motor[RIGHT].setVoltage(speedRight / 51.0);
            }
            else
            {
                robot.motor[RIGHT].setVoltage(-speedRight / 51.0);
            }
            break;
        }
        }
    }
}