#include <Arduino.h>
#include <SPI.h>
#include "dogm204.h"
#include "flash.h"

#define DOGM_204W_CONT 0x80
#define DOGM_204W_CMD 0x00
#define DOGM_204W_DATA 0x40

//--- Standard definitions for LCD ---
#define LCD_HOME_L1 0x80
#define LINE1 0
#define LINE2 LINE1 + 0x20
#define LINE3 LINE1 + 0x40
#define LINE4 LINE1 + 0x60

#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x03
#define CURSOR_ON 0x02
#define CURSOR_OFF 0x05
#define BLINK_ON 0x01
#define BLINK_OFF 0x06

#define TOPVIEW 0x05
#define BOTTOMVIEW 0x06

#define ROMA 0x00
#define ROMB 0x04
#define ROMC 0x0C

DOGM204::DOGM204(uint8_t rp)
    : resetPin(rp)
{
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
}

void DOGM204::reset()
{
  digitalWrite(resetPin, LOW);
  delay(20);
  digitalWrite(resetPin, HIGH);
  delay(20);
}

void DOGM204::init()
{
  reset();

  writeIns(0x3A); // 8-Bit data length extension Bit RE=1; REV=0
  writeIns(0x09); // 4 line display
  writeIns(0x06); // Bottom view
  writeIns(0x1E); // Bias setting BS1=1
  writeIns(0x39); // 8-Bit data length extension Bit RE=0; IS=1
  writeIns(0x1B); // BS0=1 -> Bias=1/6
  writeIns(0x6E); // Devider on and set value
  writeIns(0x57); // Booster on and set contrast (BB1=C5, DB0=C4)
  writeIns(0x72); // Set contrast (DB3-DB0=C3-C0)
  writeIns(0x38); // 8-Bit data length extension Bit RE=0; IS=0

  writeIns(0x01); // Clear display
  writeIns(0x08 | DISPLAY_ON);
  writeIns(0x80); // Set position to line 1 home
  writeIns(0x0c); // Cursor off

  setContrast(63);
  writeBytes("Azt a paszta, cipopaszta, ezt lassa meg a rendorseg!", 52);
}

void DOGM204::writeIns(uint8_t ins)
{
  SPI.beginTransaction(SPISettings(500000, LSBFIRST, SPI_MODE0));

  checkBusy();
  SPI.transfer(0x1F);              // Send 5 synchronisation bits, RS = 0, R/W = 0
  SPI.transfer(ins & 0x0F);        // send lower data bits
  SPI.transfer((ins >> 4) & 0x0F); // send higher data bits

  SPI.endTransaction();
}

void DOGM204::writeByte(uint8_t data)
{
  SPI.beginTransaction(SPISettings(500000, LSBFIRST, SPI_MODE0));

  checkBusy();
  SPI.transfer(0x5F);               // Send 5 synchronisation bits, RS = 1, R/W = 0
  SPI.transfer(data & 0x0F);        // send lower data bits
  SPI.transfer((data >> 4) & 0x0F); // send higher data bits

  SPI.endTransaction();
}

void DOGM204::writeBytes(const char *data, uint8_t len)
{
  for (; len > 0; ++data, --len)
  {
    writeByte(*data);
  }
}

void DOGM204::checkBusy()
{
  uint8_t readData = 0x80;
  while (readData & 0x80) // check for busyflag
  {
    SPI.transfer(0x3F);            // Send 5 synchronisation bits, RS = 0, R/W = 1
    readData = SPI.transfer(0x00); // dummy write to receive data
  }
}

void DOGM204::setContrast(uint8_t contr)
{
	writeIns(0x39);
	writeIns(0x54 | (contr >> 4));
	writeIns(0x70 | (contr & 0x0F));
	writeIns(0x38);
}
