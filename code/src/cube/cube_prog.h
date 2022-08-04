#ifndef CUBE_PROG_H
#define CUBE_PROG_H

#include "../prog.h"
#include "vectors.h"

class CubeProg : public Prog
{
private:
  Vec3 zOfs;
  Vec3 pts[8];
  Vec2 pts2[8];
  float persp;
  float angVeloX = 0;
  float angVeloY = 0;

private:
  Vec2 project(const Vec3 &pt);

public:
  CubeProg();
  uint16_t frame(uint32_t fc, uint8_t currentGesture);
};

#endif
