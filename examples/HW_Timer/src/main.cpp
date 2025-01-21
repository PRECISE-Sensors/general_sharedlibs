
#include "ESP_HW_Timer.h"


//Static member variables must be defined outside the class. /*required because static variables are shared across all instances of the class*/
//volatile std::atomic<bool> TimerHW::timer0_flag{false};
volatile std::atomic<bool> TimerHW::timer1_flag{false};

//Single instance of TimerHW (only one should exist)
TimerHW timerInt(1000, 1); // 1ms interval, Timer1

void setup() {
  Serial.begin(921600);
  timerInt.begin();           // Start the timer interrupt
  delay(10);
}

void loop() {
  if (TimerHW::timer1_flag) {
    TimerHW::timer1_flag = false; // Reset the flag
    Serial.println("Timer triggered");
    // Additional timer event handling
  }
}
