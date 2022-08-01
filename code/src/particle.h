#ifndef PARTICLE_H
#define PARTICLE_H

class ParticleAccessor;

class Particle
{
private:
  uint8_t x;
  uint8_t y; // If highest bit is set: updated = true

public:
  int8_t vx, vy;
  int16_t movX, movY;

private:
  bool tryMove(int8_t dx, int8_t dy, ParticleAccessor &pr);
  void move(int8_t dx, int8_t dy, ParticleAccessor &pr);

public:
  Particle();
  void update(int16_t fx, int16_t fy, ParticleAccessor &pr);
  int16_t getX();
  int16_t getY();
  void setXY(int16_t x, int16_t y);
  bool isUpdated();
  void setUpdated(bool val);
};


#endif
