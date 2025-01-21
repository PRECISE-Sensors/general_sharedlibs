
#include <Arduino.h>
#include <atomic>
#include "ESP_HW_TImer.h"

void TimerHW::begin()
{
  timer = timerBegin(timerIndex_, 80, true); // Timer index, prescaler 80, counting up
                                             // *uint16_t prescaler_val = 80;*- determines the clock division for the timer. Timer Clock Frequency = APB Clock Frequency / (prescaler_val + 1)
                                             //  to 1 MHz (80 MHz / (80 + 1) = 1 MHz)
                                             //  without a prescaler, the timer would increment every 1 / 80 MHz = 12.5 ns.
  if (timerIndex_ == 0)
  {
    timerAttachInterrupt(timer, Timer0_ISR, true);
  }
  else
  {
    timerAttachInterrupt(timer, Timer1_ISR, true);
  }
  timerAlarmWrite(timer, interval_, true); // Set the alarm value in microseconds (input must be in ms then)
  timerAlarmEnable(timer);                 // Enable the timer
}

void TimerHW::stop()
{
  timerAlarmDisable(timer);    // Stop the timer
  timerDetachInterrupt(timer); // Detach the interrupt
  timerEnd(timer);             // Deallocate the timer
}

void TimerHW::Timer0_ISR()
{
  timer0_flag = true; // Read done
}

void TimerHW::Timer1_ISR()
{
  timer1_flag = true; // Read done
}
