#ifndef GRID_H
#define GRID_H

#define N_ROWS 64
#define N_COLS 128

struct Grid
{
  Grid();
  uint8_t data[N_COLS][N_ROWS];
};

extern Grid grid;

#endif
