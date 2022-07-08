#include <Arduino.h>
#include <SPI.h>
#include "dogm204.h"
#include "flash.h"

// GND > 78 / 3c
// VCC > 7a / 3d

#define RESET_PIN 3

DOGM204 dog(RESET_PIN);

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  delay(1000);
  flash(2);
  delay(1000);

  SPI.begin();
  dog.init();
}

void loop()
{
}