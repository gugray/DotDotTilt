#ifndef VECTORS_H
#define VECTORS_H

struct Vec3
{
  float x, y, z;
  void add(const Vec3 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
  }
};

struct Vec2
{
  float x, y;
};

#endif
