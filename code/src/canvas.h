#ifndef CANVAS_H
#define CANVAS_H

#define N_ROWS 64
#define N_COLS 128

struct Canvas
{
  bool dirty = true;
  uint8_t pixels[128 * 8];

  Canvas();
  void clear();
  // Draws text in 6x8 font, aligned to 8-pixel line.
  void fwText(uint8_t x, uint8_t line, const char *str);
  void setPixel(uint8_t x, uint8_t y, bool on);
  void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
};

#endif
