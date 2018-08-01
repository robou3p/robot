#ifndef ROSINTERFACE_H
#define ROSINTERFACE_H

#include "Arduino.h"

class ROSInterface
{
public:
  void subscribe();
  void publishBatteryState();
  void publishOdometry();
  void publishJointState();
  void publishIMU();
  void publishMagneticField();

private:
  uint8_t
  checksum(uint8_t *buffer, uint8_t size);
  size_t cobs_encode(const uint8_t *input, size_t length, uint8_t *output);
  size_t cobs_decode(const uint8_t *input, size_t length, uint8_t *output);
  void publish(char topic, float value);
  void publish(char topic, float value1, float value2, float value3);
  void publish(char topic, float value1, float value2, float value3, float value4);
  void publish(char topic, float *values, uint8_t size);

  uint8_t input_buffer[20];
  uint8_t input_buffer_index = 0;
  uint8_t output_buffer[20];
};

#endif