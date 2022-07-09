#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle
{
  static uint16_t nextId;
  static uint16_t nParticles;

  uint16_t id;
  uint8_t x, y;
  uint16_t vx, vy;
  uint16_t movX, movY;
  bool updated;

  static void init(uint16_t nParticles);
  void update(uint8_t fx, uint8_t fy);

private:
  Particle();
  bool tryMove(uint8_t dx, uint8_t dy);
  void move(uint8_t dx, uint8_t dy);
};

extern Particle *particles;

#endif
