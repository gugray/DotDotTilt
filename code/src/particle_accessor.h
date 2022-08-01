#ifndef PARTICLE_ACCESSOR_H
#define PARTICLE_ACCESSOR_H

class Particle;

class ParticleAccessor
{
public:
  virtual Particle *getParticle(uint8_t indicator, uint8_t x, uint8_t y) = 0;
  virtual uint8_t getGridAt(uint8_t x, uint8_t y) = 0;
  virtual void setGridAt(uint8_t x, uint8_t y, uint8_t val) = 0;
};

#endif
