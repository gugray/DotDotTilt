#include <Arduino.h>
#include "globals.h"
#include "particle.h"
#include "particle_accessor.h"

Particle::Particle()
    : x(0), y(0), vx(0), vy(0), movX(0), movY(0)
{
}

int8_t add(int8_t v, int16_t f)
{
  f = f + v;
  if (f > 127)
    f = 127;
  else if (f < -127)
    f = -127;
  return f;
}

int8_t brake(int8_t v)
{
  int16_t q = v * 95;
  return q / 100;
}

void Particle::update(int16_t fx, int16_t fy, ParticleAccessor &pr)
{
  if (isUpdated())
    return;
  setUpdated(true);

  // Add force to velocity (scaled); friction
  vx = brake(vx);
  vy = brake(vy);
  vx = add(vx, fx);
  vy = add(vy, fy);

  // if (vx + vy > 0)
  // {
  //   let noiseLevel = Math.sqrt(this.vx * *2, this.vy * *2) * 0.1;
  //   this.vx += random(-noiseLevel, noiseLevel);
  //   this.vy += random(-noiseLevel, noiseLevel);
  // }
  // vx = vx * 9 / 10;
  // vy = vy * 9 / 10;

  // Add velocity to cumulative movement
  // If either reaches 1, we move
  movX += vx;
  movY += vy;
  int8_t dx = movX / 100;
  int8_t dy = movY / 100;

  if (dx == 0 && dy == 0)
    return;

  // Test new position. We can be kicking particle there,
  // But only if it hasn't been updated own yet.
  if (tryMove(dx, dy, pr))
  {
    move(dx, dy, pr);
  }
  // If we cannot move there: we come to a halt
  else
  {
    if (dx != 0)
    {
      vx = 0;
      movX = 0;
    }
    if (dy != 0)
    {
      vy = 0;
      movY = 0;
    }
  }
}

int16_t Particle::getX()
{
  return x;
}

int16_t Particle::getY()
{
  return (int16_t)(y & 0x7f);
}

void Particle::setXY(int16_t x, int16_t y)
{
  this->x = x;
  this->y = y;
}

bool Particle::isUpdated()
{
  return (y & 0x80) != 0;
}

void Particle::setUpdated(bool val)
{
  if (val)
    y |= 0x80;
  else
    y &= 0x7f;
}

bool Particle::tryMove(int8_t dx, int8_t dy, ParticleAccessor &pr)
{
  int16_t cx = getX();
  int16_t cy = getY();

  int16_t trgX = cx + dx,
          trgY = cy + dy;
  if (trgX < 0 || trgX >= N_COLS)
    return false;
  if (trgY < 0 || trgY >= N_ROWS)
    return false;
  uint8_t indicator = pr.getGridAt(trgX, trgY);
  if (indicator == 0)
    return true;
  Particle *other = pr.getParticle(indicator, trgX, trgY);
  if (other->isUpdated())
    return false;
  int16_t otherTrgX = other->getX() + dx;
  int16_t otherTrgY = other->getY() + dy;
  if (otherTrgX < 0 || otherTrgX >= N_COLS)
    return false;
  if (otherTrgY < 0 || otherTrgY >= N_ROWS)
    return false;
  if (pr.getGridAt(otherTrgX, otherTrgY) != 0)
    return false;
  other->move(dx, dy, pr);
  other->setUpdated(true);
  return true;
}

void Particle::move(int8_t dx, int8_t dy, ParticleAccessor &pr)
{
  bool updated = isUpdated();
  int16_t cx = getX();
  int16_t cy = getY();

  uint8_t indicator = pr.getGridAt(cx, cy);
  pr.setGridAt(cx, cy, 0);

  x = cx + dx;
  y = cy + dy;
  pr.setGridAt(x, y, indicator);

  if (updated)
    y |= 0x80;

  movX -= (int16_t)dx * 100;
  movY -= (int16_t)dy * 100;
}
