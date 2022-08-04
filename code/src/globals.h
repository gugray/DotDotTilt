#ifndef GLOBALS_H
#define GLOBALS_H

#include <DOG7565R.h>
#include <MPU6050_light.h>
#include "gestures.h"
#include "canvas.h"

#define BUF_SZ 32

/* Externs are defined in main.cpp */
/* ------------------------------- */
extern char buf[BUF_SZ];
extern DOG7565R dog;
extern MPU6050 mpu;
extern Gestures gestures;
extern Canvas canvas;

void flushCanvasToDisplay();

#endif
