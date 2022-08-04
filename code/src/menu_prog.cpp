#include <Arduino.h>
#include "menu_prog.h"
#include "globals.h"

MenuProg::MenuProg()
{
}

void MenuProg::addItem(const MenuItem &item)
{
  items.push_back(item);
}

void drawArrow(uint8_t y)
{
  canvas.line(6, y, 17, y);
  canvas.line(10, y - 2, 17, y);
  canvas.line(10, y + 2, 17, y);
}

uint16_t MenuProg::frame(uint32_t fc, uint8_t currentGesture)
{
  canvas.clear();

  for (uint8_t i = 0; i < items.size(); ++i)
  {
    canvas.fwText(20, 1 + i * 2, items[i].text);
  }
  if (animState == None || animState == WaitingForFlat)
    drawArrow(11 + currIx * 16);
  else
  {
    float arrowY = 11 + currIx * 16;
    float sn = animState == Up ? -1 : 1;
    arrowY += 16 * sn * animVal;
    arrowY = round(arrowY);
    drawArrow((uint8_t)arrowY);
  }

  if (animState == None)
  {
    if (mpu.getAngleY() > 5 && currIx < items.size() - 1)
      animState = Down;
    if (mpu.getAngleY() < -5 && currIx > 0)
      animState = Up;
    animVal = 0;
  }
  else if (animState == WaitingForFlat)
  {
    if (abs(mpu.getAngleY()) < 5)
      animState = None;
  }
  else
  {
    animVal = (animVal + 0.05) * 1.07;
    if (animVal >= 1)
    {
      animVal = 0;
      if (animState == Up)
        currIx -= 1;
      else if (animState == Down)
        currIx += 1;
      animState = WaitingForFlat;
    }
  }

  if (animState == None && currentGesture == GESTURE_FLIP_RIGHT)
    return items[currIx].code;
  else
    return PRG_CONTINUE;
}

// 35248