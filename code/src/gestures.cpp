#include <Arduino.h>
#include <MPU6050_light.h>
#include "gestures.h"

#define FLIP_THRESHOLD 15

Gestures::Gestures(MPU6050 *mpu)
{
  this->mpu = mpu;
}

void Gestures::frame()
{
  int16_t angleX = mpu->getAngleX() * 10.0F;

  for (int8_t i = GESTURES_N_LAST_ANGLES - 2; i >= 0; --i)
    lastAngleX[i + 1] = lastAngleX[i];
  lastAngleX[0] = angleX;

  if (lastAngleX[1] - lastAngleX[3] > FLIP_THRESHOLD && angleX - lastAngleX[1] < -FLIP_THRESHOLD)
    curr = GESTURE_FLIP_RIGHT;
  else if (lastAngleX[1] - lastAngleX[3] < -FLIP_THRESHOLD && angleX - lastAngleX[1] > FLIP_THRESHOLD)
    curr = GESTURE_FLIP_LEFT;
  else
    curr = 0;
}

uint8_t Gestures::currentGesture()
{
  return curr;
}
