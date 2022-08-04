#ifndef GESTURES_H
#define GESTURES_H

#define GESTURE_FLIP_LEFT 1
#define GESTURE_FLIP_RIGHT 2

#define GESTURES_N_LAST_ANGLES 5

class MPU6050;

class Gestures
{
private:
  MPU6050 *mpu;
  uint8_t curr = 0;
  int16_t lastAngleX[GESTURES_N_LAST_ANGLES];

public:
  Gestures(MPU6050 *mpu);
  void frame();
  uint8_t currentGesture();
};

#endif
