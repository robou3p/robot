#include "Bluetooth.h"
#include "Robot.h"

/**
 * @brief Renames the Bluetooth module by sending a sequence of AT commands to the RN42 module. The module stays renames after reset.
 * 
 * @param name Name of the Bluetooth module.
 */
void Bluetooth::rename(const char *name)
{
    Serial1.write("$$$");
    delay(500);
    Serial1.write("SN,");
    for (int i = 0; i < strlen(name); i++)
    {
        Serial1.write(name[i]);
    }
    Serial1.write("\n");
    delay(500);
    Serial1.write("R,1\n");
}

/**
 * @brief Enables control of the robot using the corresponding mobile phone application. This function is to be repeatedly called within loop().
 * 
 */
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
                robot.motor[LEFT].setVoltage(speedLeft / 50.0);
            }
            else
            {
                robot.motor[LEFT].setVoltage(-speedLeft / 50.0);
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
                robot.motor[RIGHT].setVoltage(speedRight / 50.0);
            }
            else
            {
                robot.motor[RIGHT].setVoltage(-speedRight / 50.0);
            }
            break;
        }
        }
    }
}