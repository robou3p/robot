#include "ROSInterface.h"
#include "Robot.h"

uint8_t ROSInterface::checksum(uint8_t *buffer, uint8_t size)
{
  uint8_t chksum = 0;
  for (uint8_t i = 0; i < size; i++)
  {
    chksum ^= buffer[i];
  }
  return chksum;
}

// COBS code is taken from here: https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c
size_t ROSInterface::cobs_encode(const uint8_t *input, size_t length, uint8_t *output)
{
  size_t read_index = 0;
  size_t write_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;
  while (read_index < length)
  {
    if (input[read_index] == 0)
    {
      output[code_index] = code;
      code = 1;
      code_index = write_index++;
      read_index++;
    }
    else
    {
      output[write_index++] = input[read_index++];
      code++;
      if (code == 0xFF)
      {
        output[code_index] = code;
        code = 1;
        code_index = write_index++;
      }
    }
  }
  output[code_index] = code;
  return write_index;
}

size_t ROSInterface::cobs_decode(const uint8_t *input, size_t length, uint8_t *output)
{
  size_t read_index = 0;
  size_t write_index = 0;
  uint8_t code;
  uint8_t i;
  while (read_index < length)
  {
    code = input[read_index];
    if (read_index + code > length && code != 1)
    {
      return 0;
    }
    read_index++;
    for (i = 1; i < code; i++)
    {
      output[write_index++] = input[read_index++];
    }
    if (code != 0xFF && read_index != length)
    {
      output[write_index++] = '\0';
    }
  }
  return write_index;
}

void ROSInterface::publish(char topic, float value)
{
  input_buffer[0] = topic;
  uint8_t *data = (uint8_t *)&value; // get access to the float as a byte-array
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 1] = data[i];
  }
  input_buffer[5] = checksum(input_buffer, 5);
  uint8_t length = cobs_encode(input_buffer, 6, output_buffer);
  output_buffer[length] = 0;
  for (uint8_t i = 0; i < length + 1; i++)
  {
    Serial1.write(output_buffer[i]);
  }
}

void ROSInterface::publish(char topic, float value1, float value2, float value3)
{
  input_buffer[0] = topic;
  uint8_t *data = (uint8_t *)&value1; // get access to the float as a byte-array
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 1] = data[i];
  }
  data = (uint8_t *)&value2;
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 5] = data[i];
  }
  data = (uint8_t *)&value3;
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 9] = data[i];
  }
  input_buffer[13] = checksum(input_buffer, 13);
  uint8_t length = cobs_encode(input_buffer, 14, output_buffer);
  output_buffer[length] = 0;
  for (uint8_t i = 0; i < length + 1; i++)
  {
    Serial1.write(output_buffer[i]);
  }
}

void ROSInterface::publish(char topic, float value1, float value2, float value3, float value4)
{
  input_buffer[0] = topic;
  uint8_t *data = (uint8_t *)&value1; // get access to the float as a byte-array
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 1] = data[i];
  }
  data = (uint8_t *)&value2;
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 5] = data[i];
  }
  data = (uint8_t *)&value3;
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 9] = data[i];
  }
  data = (uint8_t *)&value4;
  for (uint8_t i = 0; i < 4; i++)
  {
    input_buffer[i + 13] = data[i];
  }
  input_buffer[17] = checksum(input_buffer, 17);
  uint8_t length = cobs_encode(input_buffer, 18, output_buffer);
  output_buffer[length] = 0;
  for (uint8_t i = 0; i < length + 1; i++)
  {
    Serial1.write(output_buffer[i]);
  }
}

void ROSInterface::publish(char topic, float *values, uint8_t size)
{
  // in this case, we will allocate a buffer on stack
  uint8_t input_buffer[60];
  uint8_t output_buffer[60];
  input_buffer[0] = topic;
  uint8_t j = 0;
  while (j < size)
  {
    uint8_t *data = (uint8_t *)&(values[j]); // get access to the float as a byte-array
    for (uint8_t i = 0; i < 4; i++)
    {
      input_buffer[i + j * 4 + 1] = data[i];
    }
    j++;
  }
  input_buffer[j * 4 + 1] = checksum(input_buffer, j * 4 + 1);
  uint8_t length = cobs_encode(input_buffer, j * 4 + 2, output_buffer);
  output_buffer[length] = 0;
  for (uint8_t i = 0; i < length + 1; i++)
  {
    Serial1.write(output_buffer[i]);
  }
}

void ROSInterface::subscribe()
{
  if (Serial1.available())
  {
    uint8_t read_byte = Serial1.read();
    // read bytes into buffer
    if (read_byte != 0)
    {
      input_buffer[input_buffer_index++] = read_byte;
    }
    // process buffer
    else
    {
      // cobs decode
      uint8_t length = cobs_decode(input_buffer, input_buffer_index, output_buffer);
      // reset index
      input_buffer_index = 0;
      if (length == 0)
      {
        // error 0
      }
      else if (checksum(output_buffer, length) != 0)
      {
        // error 1
      }
      else
      {
        switch (output_buffer[0])
        {
        // left wheel voltage
        case 'L':
        {
          float voltage;
          uint8_t *data = (uint8_t *)&voltage;
          for (uint8_t i = 0; i < 4; i++)
          {
            data[i] = output_buffer[i + 1];
          }
          robot.motor[LEFT].setVoltage(voltage);
          break;
        }
        // right wheel voltage
        case 'R':
        {
          float voltage;
          uint8_t *data = (uint8_t *)&voltage;
          for (uint8_t i = 0; i < 4; i++)
          {
            data[i] = output_buffer[i + 1];
          }
          robot.motor[RIGHT].setVoltage(voltage);
          break;
        }
        // twist command
        case 'T':
        {
          float v, w;
          uint8_t *data = (uint8_t *)&v;
          for (uint8_t i = 0; i < 4; i++)
          {
            data[i] = output_buffer[i + 1];
          }
          data = (uint8_t *)&w;
          for (uint8_t i = 0; i < 4; i++)
          {
            data[i] = output_buffer[i + 5];
          }
          robot.drive(v, w);
          break;
        }
        // button pressed
        case 'B':
        {
          robot.button.press();
          break;
        }
        }
      }
    }
  }
}

void ROSInterface::publishBatteryState()
{
  publish('B', robot.battery.getVoltage());
}

void ROSInterface::publishOdometry()
{
  publish('O', robot.motor[LEFT].getDistance(), robot.motor[RIGHT].getDistance(), robot.getSpeed(), robot.getAngularVelocity());
}

void ROSInterface::publishJointState()
{
  publish('J', robot.motor[LEFT].getDistanceRad(), robot.motor[RIGHT].getDistanceRad(), robot.motor[LEFT].getSpeed(), robot.motor[RIGHT].getSpeed());
}

void ROSInterface::publishIMU()
{
  float imu_data[] = {robot.imu.filters.q[0], robot.imu.filters.q[1], robot.imu.filters.q[2], robot.imu.filters.q[3], robot.imu.mpu._gx, robot.imu.mpu._gy, robot.imu.mpu._gz, robot.imu.mpu._ax, robot.imu.mpu._ay, robot.imu.mpu._az};
  publish('I', imu_data, 10);
}

void ROSInterface::publishMagneticField()
{
  publish('M', robot.imu.getMX(), robot.imu.getMY(), robot.imu.getMZ());
}