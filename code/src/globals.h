#ifndef GLOBALS_H
#define GLOBALS_H

#include <DOG7565R.h>
#include <MPU6050_light.h>
#include "canvas.h"

/* Globals are defined in main.cpp */

#define BUF_SZ 32

extern bool progRunning;
extern char buf[BUF_SZ];
extern DOG7565R dog;
extern MPU6050 mpu;
extern Canvas canvas;

void flushCanvasToDisplay();

#endif
