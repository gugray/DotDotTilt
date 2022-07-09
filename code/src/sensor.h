#ifndef SENSOR_H
#define SENSOR_H

// Based on:
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/

class Sensor
{
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
  float accErrorX, accErrorY, gyroErrorX, gyroErrorY, gyroErrorZ;
  float elapsedTime, currentTime, previousTime;

public:
  float roll, pitch, yaw;

  Sensor();
  void init();
  void calibrate();
  void update();
};

#endif
