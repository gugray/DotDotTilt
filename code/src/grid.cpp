#include <Arduino.h>
#include "grid.h"

Grid grid;

Grid::Grid()
{
  for (uint8_t x = 0; x < N_COLS; ++x)
  {
    for (uint8_t y = 0; y < N_ROWS; ++y)
    {
      data[x][y] = 0;
    }
  }
}
