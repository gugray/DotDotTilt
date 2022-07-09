#include <Arduino.h>
#include <SPI.h>
#include <dog_7565R.h>
#include <font_16x32nums.h>
#include <font_6x8.h>
#include <font_8x16.h>
#include <font_8x8.h>
#include <logo_BLH.h>

#include "flash.h"

#define RESET_PIN 3
#define CS_PIN 4
#define A0_PIN 5

dog_7565R DOG;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // SI=CS => Hardware SPI
  DOG.initialize(CS_PIN, 0, 0, A0_PIN, RESET_PIN, DOGM128);

  DOG.clear(); // clear whole display
  DOG.picture(0, 0, ea_logo);
  DOG.string(71, 0, font_8x16, "DOGM128");
  DOG.rectangle(71, 2, 127, 2, 0x03);
  DOG.string(0, 3, font_6x8, "-3.3V single supply");
  DOG.string(0, 4, font_6x8, "-ST7565R controller");
  DOG.string(0, 5, font_6x8, "-different backlights");
  DOG.string(0, 6, font_6x8, "-extreme low power");
  DOG.string(0, 7, font_8x8, "lcd-module.com");
}

void loop()
{

  flash(3);
  delay(1000);
}