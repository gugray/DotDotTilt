#include <Arduino.h>
#include <SPI.h>
#include <DOG7565R.h>
#include <font_6x8.h>
#include "grid.h"
#include "particle.h"
#include "sensor.h"

#define N_PARTICLES 512

#define RESET_PIN 3
#define CS_PIN 4
#define A0_PIN 5

DOG7565R dog;
Sensor sensor;
uint8_t fx = 0, fy = 0;

void flash(uint16_t delayAfter = 200)
{
  digitalWrite(PIN_LED, LOW);
  delay(20);
  digitalWrite(PIN_LED, HIGH);
  delay(delayAfter);
}

void setup()
{
  digitalWrite(PIN_LED, HIGH);
  pinMode(PIN_LED, OUTPUT);

  dog.initialize(CS_PIN, A0_PIN, RESET_PIN, DOGM128);

  dog.clear();
  dog.rectangle(71, 2, 127, 2, 0b10101001);

  delay(1000);
  Particle::init(512);

  dog.string(0, 3, font_6x8, "Sand Simulator");
  dog.string(0, 4, font_6x8, "Calibrating...");
  sensor.calibrate();
  flash();

  delay(1000);
}

void redraw()
{
  for (uint8_t p = 0; p < 8; ++p)
  {
    dog.position(0, p);
    for (uint8_t x = 0; x < 128; ++x)
    {
      uint8_t val = 0;
      uint8_t y = p * 8;
      for (uint8_t ofs = 0; ofs < 8; ++ofs)
      {
        if (grid.data[x][y + ofs] != 0)
          val += (1 << ofs);
      }
      dog.data(val);
    }
  }
}

void loop()
{
  sensor.update();
  for (uint16_t i = 0; i < Particle::nParticles; ++i)
    particles[i].update(fx, fy);
  for (uint16_t i = 0; i < Particle::nParticles; ++i)
    particles[i].update(fx, fy);
  redraw();
  delay(60);
}