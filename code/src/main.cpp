#include <Arduino.h>
#include <SPI.h>
#include <DOG7565R.h>
#include <font_6x8.h>
#include <MPU6050_light.h>
#include "grid.h"
#include "particle.h"

// TO-DO
// -- single-byte vx, vy and movX, movY
// -- cache last screen; leave trace

#define N_PARTICLES 2048
#define F_ANGLE_MUL 5

#define RESET_PIN 3
#define CS_PIN 1
#define A0_PIN 2

DOG7565R dog;
MPU6050 mpu(Wire);

int16_t fx = -40, fy = 60;

#define STRLEN 32
char str[STRLEN];
const char *msgAngleX = "X angle: %3d";
const char *msgAngleY = "Y angle: %3d";

uint8_t pixelData[128 * 8];

void flash(uint16_t delayAfter = 200)
{
  digitalWrite(PIN_LED, LOW);
  delay(50);
  digitalWrite(PIN_LED, HIGH);
  delay(delayAfter);
}

void redraw()
{
  uint16_t i = 0;
  bool anyChange = false;
  for (uint8_t p = 0; p < 8; ++p)
  {
    for (uint8_t x = 0; x < 128; ++x, ++i)
    {
      uint8_t val = 0;
      uint8_t y = p * 8;
      for (uint8_t ofs = 0; ofs < 8; ++ofs)
      {
        if (grid.data[x][y + ofs] != 0)
          val += (1 << ofs);
      }
      uint8_t last = pixelData[i];
      if (last != val)
      {
        anyChange = true;
        pixelData[i] = val;
      }
    }
  }

  if (anyChange)
  {
    i = 0;
    for (uint8_t p = 0; p < 8; ++p)
    {
      dog.position(0, p);
      for (uint8_t x = 0; x < 128; ++x, ++i)
      {
        dog.data(pixelData[i]);
      }
    }    
  }
}

void setup()
{
  digitalWrite(PIN_LED, HIGH);
  pinMode(PIN_LED, OUTPUT);

  dog.initialize(CS_PIN, A0_PIN, RESET_PIN, DOGM128);
  Particle::init(N_PARTICLES);

  Wire.begin();
  mpu.begin();

  dog.string(0, 3, font_6x8, "Calibrating");
  delay(1000);
  mpu.calcOffsets(true, true);
  delay(1000);
  mpu.calcOffsets(true, true);

  uint16_t i = 0;
  for (uint8_t p = 0; p < 8; ++p)
    for (uint8_t x = 0; x < 128; ++x, ++i)
      pixelData[i] = 0;

  redraw();
}


void loop()
{
  mpu.update();

  // snprintf(str, STRLEN, msgAngleX, (int16_t)mpu.getAngleX());
  // dog.string(0, 3, font_6x8, str);
  // snprintf(str, STRLEN, msgAngleY, (int16_t)mpu.getAngleY());
  // dog.string(0, 4, font_6x8, str);

  fx = -F_ANGLE_MUL * (int16_t)mpu.getAngleX();
  fy = F_ANGLE_MUL * (int16_t)mpu.getAngleY();

  for (uint16_t i = 0; i < Particle::nParticles; ++i)
    Particle::particles[i].update(fx, fy);
  for (uint16_t i = 0; i < Particle::nParticles; ++i)
    Particle::particles[i].setUpdated(false);
  redraw();
  delay(60);
}