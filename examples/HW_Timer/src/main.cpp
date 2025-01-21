
#include "ESP_HW_Timer.h"

TimerHW timer(1000, 1); // 1ms interval, Timer1

void setup() {
  Serial.begin(115200);
  timer.begin(); // Start the timer
}

void loop() {
  if (TimerHW::timer1_flag) {
    TimerHW::timer1_flag = false; // Reset the flag
    Serial.println("Timer triggered");
    // Additional timer event handling
  }
}
