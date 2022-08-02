#ifndef SAND_PROG_H
#define SAND_PROG_H

#include "../globals.h"
#include "../prog.h"
#include "particle_accessor.h"


class Particle;

class SandProg : public Prog, ParticleAccessor
{
private:
  uint8_t grid[N_COLS][N_ROWS];
  uint16_t nParticles;
  Particle *particles;

private:
  Particle *getParticle(uint8_t indicator, uint8_t x, uint8_t y);
  uint8_t getGridAt(uint8_t x, uint8_t y);
  void setGridAt(uint8_t x, uint8_t y, uint8_t val);
  void redraw();

public:
  SandProg();
  ~SandProg();
  uint16_t frame(uint32_t fc);
};

#endif
