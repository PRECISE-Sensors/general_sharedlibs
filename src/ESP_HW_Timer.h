

#ifndef  ESP_HW_TIMER_H
#define  ESP_HW_TIMER_H

#include <Arduino.h>
#include <atomic>


class TimerHW
{
public:
  /**
   *Interval (us), Index: there are two hardware timers: Timer0 and Timer1. Timer1 is recommended for user applications */
  TimerHW(uint32_t interval, uint8_t timerIndex = 1) : interval_(interval), timerIndex_(timerIndex) {}
  static volatile std::atomic<bool> timer0_flag;
  static volatile std::atomic<bool> timer1_flag;

  /* uint64_t interval = 1000; // in micros ; 1 ms for 1 kHz sampling; 1250* 0.000001 = 1.25ms  (interval, index of the timer 1 o r 0)*/

  void begin();
  void stop();

private:
  static void Timer0_ISR();
  static void Timer1_ISR();

  hw_timer_t *timer; //  declare and define a hardware timer (handle)
  uint32_t interval_;
  uint8_t timerIndex_;
};

#endif