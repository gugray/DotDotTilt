#ifndef PROG_SAND_H
#define PROG_SAND_H

#include "globals.h"
#include "prog.h"
#include "particle_accessor.h"


class Particle;

class SandProg : public Prog, ParticleAccessor
{
private:
  uint8_t pixelData[128 * 8];
  uint8_t grid[N_COLS][N_ROWS];
  uint16_t nParticles;
  Particle *particles;

public:
  int8_t simActive = 0;

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
