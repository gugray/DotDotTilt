#include <Arduino.h>
#include "../globals.h"
#include "particle.h"
#include "sand_prog.h"

#define N_PARTICLES 2048
#define F_ANGLE_MUL 5

int16_t fx = 0, fy = 0;

SandProg::SandProg()
{
  for (uint8_t x = 0; x < N_COLS; ++x)
  {
    for (uint8_t y = 0; y < N_ROWS; ++y)
    {
      grid[x][y] = 0;
    }
  }

  nParticles = N_PARTICLES;
  particles = new Particle[nParticles];
  for (uint16_t i = 0; i < nParticles; ++i)
  {
    uint8_t x, y;
    while (true)
    {
      x = random(N_COLS);
      y = random(N_ROWS);
      if (grid[x][y] == 0)
        break;
    }
    Particle &p = particles[i];
    p.setXY(x, y);
    grid[x][y] = 0x80 + (i % 128);
  }
}

SandProg::~SandProg()
{
  delete particles;
}

Particle *SandProg::getParticle(uint8_t indicator, uint8_t x, uint8_t y)
{
  indicator &= 0x7f;
  Particle *res = NULL;
  for (uint16_t ofs = indicator; ofs < nParticles; ofs += 128)
  {
    res = particles + ofs;
    if (res->getX() == x && res->getY() == y)
      break;
  }
  return res;
}

uint8_t SandProg::getGridAt(uint8_t x, uint8_t y)
{
  return grid[x][y];
}

void SandProg::setGridAt(uint8_t x, uint8_t y, uint8_t val)
{
  grid[x][y] = val;
}

void SandProg::redraw()
{
  uint16_t i = 0;
  for (uint8_t p = 0; p < 8; ++p)
  {
    for (uint8_t x = 0; x < 128; ++x, ++i)
    {
      uint8_t val = 0;
      uint8_t y = p * 8;
      for (uint8_t ofs = 0; ofs < 8; ++ofs)
      {
        if (grid[x][y + ofs] != 0)
          val += (1 << ofs);
      }
      uint8_t last = canvas.pixels[i];
      if (last != val)
      {
        canvas.dirty = true;
        canvas.pixels[i] = val;
      }
    }
  }
}

uint16_t SandProg::frame(uint32_t fc, uint8_t currentGesture)
{
  int16_t accZ = (int16_t)(mpu.getAccZ() * 100.0F);
  int16_t angleX = mpu.getAngleX() * 10.0F;
  int16_t angleY = mpu.getAngleY() * 10.0F;

  if (fc % 2 == 0)
  {
    fx = F_ANGLE_MUL * angleX / 10;
    fy = F_ANGLE_MUL * angleY / 10;
    int16_t minAngle = 3;
    if (angleX >= -minAngle && angleX <= minAngle)
      fx = 0;
    if (angleY >= -minAngle && angleY <= minAngle)
      fy = 0;

    for (uint16_t i = 0; i < nParticles; ++i)
      particles[i].update(fx, fy, *this);
    for (uint16_t i = 0; i < nParticles; ++i)
      particles[i].setUpdated(false);
    redraw();
  }

  // sprintf(msg, "X:  %4d   Y: %4d  ", angleX, angleY);
  // dog.string(0, 2, font_6x8, msg);
  // Serial.printf("AngleX: %4d\tAngleY: %4d\tAccZ: %4d\n", angleX, angleY, (int16_t)(mpu.getAccZ() * 100.0F));
  return PRG_CONTINUE;
}
