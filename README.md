# smartiwear_sharedlibs

This is the library containing modules/classes to support the same functionality across the various sensors created under the **Smart Garment** and **BioSpine** projects.

## Modules
- **Wireless Communication Module**: Handles ESP-NOW communication and configuration.
- **I2C Module**: Manages communication over the I2C protocol.
- **Hardware Timer Module**: Provides reusable timer functions.
  ESP_HW_Timer.h , ESP_HW_Timer.cpp

## HW Timer Module

The **Timer Functionality Module** offers a simple interface for managing hardware timers on ESP boards. It supports two hardware timers (Timer0 and Timer1) and allows precise timing control for tasks such as sampling or periodic actions. 

### Features:
- **Customizable Timer Intervals**: Specify the interval in microseconds for high-resolution timing.
- **Two Timers Supported**: Choose between Timer0 or Timer1 for flexibility in resource allocation.
- **Atomic Flags**: Provides thread-safe, atomic flags (`timer0_flag` and `timer1_flag`) to signal timer events.
- **Begin and Stop**: Easy-to-use methods to start (`begin`) and stop (`stop`) the timer.
- **ISR Handling**: Built-in interrupt service routines (ISRs) to handle timer events efficiently.


## Wireless Communication Module

The **Wireless Communication Module** simplifies the setup and operation of ESP-NOW communication for ESP boards. 
### Features:
- **Configurable Parameters**: Set WiFi channel, data rate, and optional peer MAC addresses for optimized communication.
- **Broadcast Support**: Utilizes a default broadcast address or custom MAC addresses for targeted communication.
- **Template-Based Data Transmission**: Supports sending any data type, ensuring flexibility for diverse use cases.
- **ESP-NOW Initialization**: Configures ESP-NOW communication and WiFi settings efficiently.
### Important Notes:
- **ESP-NOW Data Rates**: You can choose a specific data rate for communication. For detailed information, refer to the [ESP-IDF Data Rates Documentation](https://github.com/espressif/esp-idf/blob/625bd5eb1806809ff3cc010ee20d1f750aa778a1/components/esp_wifi/include/esp_wifi_types.h#L474).
- **WiFi Channel**: Ensure the WiFi channel is consistent across all communicating devices.
- **Data Consistency**: The data structure of the transmitted packet must be identical between the transmitter and receiver.
- **Broadcast vs Peer-to-Peer**: By default, ESP-NOW uses a broadcast address. You can specify a peer MAC address for targeted communication.

---
