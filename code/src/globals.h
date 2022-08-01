#ifndef GLOBALS_H
#define GLOBALS_H

#include <DOG7565R.h>
#include <MPU6050_light.h>
#include "canvas.h"

extern DOG7565R dog;
extern MPU6050 mpu;
extern Canvas canvas;

void flushCanvasToDisplay();

#endif
