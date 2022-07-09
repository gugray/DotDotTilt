#include <Arduino.h>
#include "particle.h"
#include "grid.h"

Particle *particles;

uint16_t Particle::nParticles = 0;
uint16_t Particle::nextId = 1;

void Particle::init(uint16_t nParticles_)
{
  nParticles = nParticles_;
  particles = new Particle[nParticles];
  for (uint16_t i = 0; i < nParticles; ++i)
  {
    uint8_t x, y;
    while (true)
    {
      x = random(N_COLS);
      y = random(N_ROWS);
      if (grid.data[x][y] == 0)
        break;
    }
    Particle &p = particles[i];
    p.x = x;
    p.y = y;
    grid.data[x][y] = p.id;
  }
}

Particle::Particle()
    : id(nextId++), x(0), y(0), vx(0), vy(0), movX(0), movY(0), updated(false)
{
}

void Particle::update(uint8_t fx, uint8_t fy)
{
  updated = true;

  // Add force to velocity (scaled); friction
  vx += fx / 20;
  vy += fy / 20;
  // if (vx + vy > 0)
  // {
  //   let noiseLevel = Math.sqrt(this.vx * *2, this.vy * *2) * 0.1;
  //   this.vx += random(-noiseLevel, noiseLevel);
  //   this.vy += random(-noiseLevel, noiseLevel);
  // }
  vx = vx * 9 / 10;
  vy = vx * 9 / 10;

  // Add velocity to cumulative movement
  // If either reaches 1, we move
  movX += vx;
  movY += vy;
  uint8_t dx = movX / 20;
  uint8_t dy = movY / 20;

  if (dx == 0 && dy == 0)
    return;

  // Test new position. We can be kicking particle there,
  // But only if it hasn't been updated own yet.
  if (tryMove(dx, dy))
  {
    move(dx, dy);
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

bool Particle::tryMove(uint8_t dx, uint8_t dy)
{
  uint8_t trgX = x + dx,
          trgY = y + dy;
  if (trgX < 0 || trgX >= N_COLS)
    return false;
  if (trgY < 0 || trgY >= N_ROWS)
    return false;
  uint16_t idOther = grid.data[trgX][trgY];
  if (idOther == 0)
    return true;
  Particle &other = particles[idOther];
  if (other.updated)
    return false;
  uint8_t otherTrgX = other.x + dx;
  uint8_t otherTrgY = other.y + dy;
  if (otherTrgX < 0 || otherTrgX >= N_COLS)
    return false;
  if (otherTrgY < 0 || otherTrgY >= N_ROWS)
    return false;
  if (grid.data[otherTrgX][otherTrgY] != 0)
    return false;
  other.move(dx, dy);
  other.updated = true;
  return true;
}

void Particle::move(uint8_t dx, uint8_t dy)
{
  grid.data[x][y] = 0;
  x += dx;
  y += dy;
  grid.data[x][y] = id;
  movX -= (uint16_t)dx * 20;
  movY -= (uint16_t)dy * 20;
}
