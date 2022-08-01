#ifndef PROG_H
#define PROG_H

#define PRG_CONTINUE      0
#define PRG_TO_MENU       1
#define PRG_TO_SAND       2
#define PRG_TO_MAZE       3

class Prog
{
public:
  virtual uint16_t frame(uint32_t fc) = 0;
};

#endif
