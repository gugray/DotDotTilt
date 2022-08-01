#include <Arduino.h>
#include <font_6x8.h>
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
SAMDTimer ITimer(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

Prog *prog = 0;


void TimerHandler()
{
  ISR_Timer.run();
}

uint32_t fc = 0;

void frame()
{
  uint32_t tStart = micros();
  mpu.update();

  uint16_t pres = prog->frame(fc);

  uint32_t tEnd = micros();

  // sprintf(msg, "%5d usec", (int16_t)(tEnd - tStart));
  // dog.string(68, 7, font_6x8, msg);

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

  dog.string(0, 3, font_6x8, "Calibrating");
  mpu.calcOffsets();
  mpu.update();

  prog = new SandProg();

  ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler);
  ISR_Timer.setInterval(16, frame);

  dog.string(11 * 6, 3, font_6x8, ".");
  for (uint8_t i = 0; i < 4; ++i)
  {
    delay(500);
    dog.string((12 + i) * 6, 3, font_6x8, ".");
  }

  ((SandProg*)prog)->simActive = 1;
}

void loop()
{
  delay(100);
}