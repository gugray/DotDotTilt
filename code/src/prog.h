#ifndef PROG_H
#define PROG_H

#define PRG_CONTINUE 0
#define PRG_RESUME 1
#define PRG_CONFIRM_QUIT 2
#define PRG_CODE_MENU 100
#define PRG_CODE_SAND 101
#define PRG_CODE_CUBE 102

class Prog
{
public:
  virtual uint16_t frame(uint32_t fc, uint8_t currentGesture) = 0;
  virtual ~Prog() {}
};

#endif
