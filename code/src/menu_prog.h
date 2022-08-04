#ifndef MENU_PROG_H
#define MENU_PROG_H

#include <vector>
#include "prog.h"

struct MenuItem
{
  const char *text;
  const uint8_t code;
};

class MenuProg : public Prog
{
private:
  enum AnimState
  {
    None,
    Down,
    Up,
    WaitingForFlat,
  };

private:
  std::vector<MenuItem> items;
  uint8_t currIx = 0;
  AnimState animState = None;
  float animVal = 0;

public:
  MenuProg();
  void addItem(const MenuItem &item);
  uint16_t frame(uint32_t fc, uint8_t currentGesture);
};

#endif
