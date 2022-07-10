#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle
{
  static uint16_t nextId;
  static uint16_t nParticles;

  uint16_t id;
  int16_t x, y;
  int16_t vx, vy;
  int16_t movX, movY;
  bool updated;

  static void init(uint16_t nParticles);
  void update(int16_t fx, int16_t fy);

private:
  Particle();
  bool tryMove(int8_t dx, int8_t dy);
  void move(int8_t dx, int8_t dy);
};

extern Particle *particles;

#endif
