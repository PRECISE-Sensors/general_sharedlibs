# smartiwear_sharedlibs

This is the library containing modules/classes to support the same functionality across the various sensors created under the **Smart Garment** and **BioSpine** projects.

## Modules
- **Wireless Communication Module**: Handles ESP-NOW communication and configuration.
- **I2C Module**: Manages communication over the I2C protocol.
- **Timer Functionality Module**: Provides reusable timer functions.
  ESP_HW_Timer.h , ESP_HW_Timer.cpp

## Timer Functionality Module

The **Timer Functionality Module** offers a simple interface for managing hardware timers on ESP boards. It supports two hardware timers (Timer0 and Timer1) and allows precise timing control for tasks such as sampling or periodic actions. 

### Features:
- **Customizable Timer Intervals**: Specify the interval in microseconds for high-resolution timing.
- **Two Timers Supported**: Choose between Timer0 or Timer1 for flexibility in resource allocation.
- **Atomic Flags**: Provides thread-safe, atomic flags (`timer0_flag` and `timer1_flag`) to signal timer events.
- **Begin and Stop**: Easy-to-use methods to start (`begin`) and stop (`stop`) the timer.
- **ISR Handling**: Built-in interrupt service routines (ISRs) to handle timer events efficiently.
