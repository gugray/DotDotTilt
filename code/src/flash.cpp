#include <Arduino.h>

void flash(uint8_t ntimes)
{
  for (uint8_t i = 0; i < ntimes; ++i)
  {
    digitalWrite(PIN_LED, LOW);
    delay(200);
    digitalWrite(PIN_LED, HIGH);
    delay(200);
  }
}