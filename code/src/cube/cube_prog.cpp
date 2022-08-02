#include <Arduino.h>
#include "../globals.h"
#include "cube_prog.h"

#define MAX_ANGVELO 0.4
#define ANGVELO_DECEL 0.99
#define ANGVELO_DAMP 0.01

CubeProg::CubeProg()
{
  zOfs = Vec3{0, 0, 1.5};
  persp = (float)N_COLS * 0.01;

  pts[0] = Vec3{-0.5, -0.5, -0.5};
  pts[1] = Vec3{0.5, -0.5, -0.5};
  pts[2] = Vec3{0.5, 0.5, -0.5};
  pts[3] = Vec3{-0.5, 0.5, -0.5};
  pts[4] = Vec3{-0.5, -0.5, 0.5};
  pts[5] = Vec3{0.5, -0.5, 0.5};
  pts[6] = Vec3{0.5, 0.5, 0.5};
  pts[7] = Vec3{-0.5, 0.5, 0.5};
}

Vec2 CubeProg::project(const Vec3 &pt)
{
  float scale = persp / (persp + pt.z);
  return Vec2{pt.x * scale, pt.y * scale};
}

void rotX(Vec3 &pt, float angle)
{
  Vec3 res = {
      pt.x,
      pt.y * cos(angle) - pt.z * sin(angle),
      pt.y * sin(angle) + pt.z * cos(angle)};
  pt = res;
}

void rotY(Vec3 &pt, float angle)
{
  Vec3 res = {
      pt.x * cos(angle) + pt.z * sin(angle),
      pt.y,
      -pt.x * sin(angle) + pt.z * cos(angle)};
  pt = res;
}

void rotZ(Vec3 &pt, float angle)
{
  Vec3 res = {
      pt.x * cos(angle) - pt.y * sin(angle),
      pt.x * sin(angle) + pt.y * cos(angle),
      pt.z};
  pt = res;
}

void updateAngVelo(float &angVelo, float change)
{
  angVelo += change;
  angVelo *= ANGVELO_DECEL;
  if (angVelo > 0)
  {
    angVelo = min(angVelo, MAX_ANGVELO);
    angVelo = max(angVelo - ANGVELO_DAMP, 0);
  }
  else
  {
    angVelo = max(angVelo, -MAX_ANGVELO);
    angVelo = min(angVelo + ANGVELO_DAMP, 0);
  }
}

uint16_t CubeProg::frame(uint32_t fc)
{
  if (!progRunning)
    return PRG_CONTINUE;

  updateAngVelo(angVeloY, -mpu.getAngleX() / 300.0F);
  updateAngVelo(angVeloX, mpu.getAngleY() / 300.0F);

  canvas.clear();

  // Project cube's corners
  for (uint8_t i = 0; i < 8; ++i)
  {
    Vec3 pt = pts[i];
    pt.add(zOfs);
    Vec2 &pt2 = pts2[i];
    pt2 = project(pt);
    pt2.x = N_COLS / 2 + pt2.x * N_ROWS;
    pt2.y = N_ROWS / 2 + pt2.y * N_ROWS;
  }

  // Draw 12 edges
  for (uint8_t i = 0; i < 4; ++i)
  {
    canvas.line(pts2[i].x, pts2[i].y, pts2[(i + 1) % 4].x, pts2[(i + 1) % 4].y);
    canvas.line(pts2[i + 4].x, pts2[i + 4].y, pts2[(i + 1) % 4 + 4].x, pts2[(i + 1) % 4 + 4].y);
    canvas.line(pts2[i].x, pts2[i].y, pts2[i + 4].x, pts2[i + 4].y);
  }

  // Rotate cube
  for (uint8_t i = 0; i < 8; ++i)
  {
    rotX(pts[i], angVeloX);
    rotY(pts[i], angVeloY);
  }
  return PRG_CONTINUE;
}
