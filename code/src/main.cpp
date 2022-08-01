#include <Arduino.h>
#include <SAMDTimerInterrupt.h>
#include <SAMD_ISR_Timer.h>
#include "globals.h"
#include "prog_sand.h"

#define RESET_PIN 3
#define CS_PIN 1
#define A0_PIN 2

#define TIMER_INTERVAL_MS 100
#define HW_TIMER_INTERVAL_MS 50
#define TIMER_INTERVAL_10_MS 10L

DOG7565R dog;
MPU6050 mpu(Wire);
Canvas canvas;
SAMDTimer ITimer(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

Prog *prog = 0;

char msg[32];

void flushCanvasToDisplay()
{
  if (!canvas.dirty)
    return;
  canvas.dirty = false;
  uint16_t i = 0;
  for (uint8_t p = 0; p < N_ROWS / 8; ++p)
  {
    dog.position(0, p);
    for (uint8_t x = 0; x < N_COLS; ++x, ++i)
    {
      dog.data(canvas.pixels[i]);
    }
  }
}

void TimerHandler()
{
  ISR_Timer.run();
}

uint32_t fc = 0;

void frame()
{
  mpu.update();

  if (prog != 0)
  {
    uint16_t pres = prog->frame(fc);
  }
  // sprintf(msg, "%5d usec", (int16_t)(tEnd - tStart));
  flushCanvasToDisplay();
  ++fc;
}

void setup()
{
  // If normal power on: let's reset!
  // Otherwise, MPU is odd an we get no Y angle
  // WTF, but it's what it is.
  if (!PM->RCAUSE.bit.SYST) // Anything but system reset
  {
    __disable_irq();
    NVIC_SystemReset();
    while (true)
      ;
  }

  digitalWrite(PIN_LED, HIGH);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);

  dog.initialize(CS_PIN, A0_PIN, RESET_PIN, DOGM128);

  Wire.begin();
  mpu.begin(0, 0);

  canvas.fwText(0, 3, "Calibrating");
  flushCanvasToDisplay();
  mpu.calcOffsets();
  mpu.update();

  prog = new SandProg();

  ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler);
  ISR_Timer.setInterval(20, frame);

  canvas.fwText(11 * 6, 3, ".");
  flushCanvasToDisplay();
  for (uint8_t i = 0; i < 4; ++i)
  {
    delay(500);
    canvas.fwText((12 + i) * 6, 3, ".");
    flushCanvasToDisplay();
  }

  ((SandProg *)prog)->simActive = 1;
}

void loop()
{
  delay(100);
}