#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
private:
  uint8_t x;
  uint8_t y; // If highest bit is set: updated = true

public:
  static uint16_t nParticles;
  static Particle *particles;
  int8_t vx, vy;
  int16_t movX, movY;

private:
  Particle();
  bool tryMove(int8_t dx, int8_t dy);
  void move(int8_t dx, int8_t dy);

public:
  static void init(uint16_t nParticles);
  static Particle *getParticle(uint8_t indicator, uint8_t x, uint8_t y);
  void update(int16_t fx, int16_t fy);
  int16_t getX();
  int16_t getY();
  bool isUpdated();
  void setUpdated(bool val);
};


#endif
