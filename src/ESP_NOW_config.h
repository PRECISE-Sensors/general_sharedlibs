#ifndef ESP_INIT_H
#define ESP_INIT_H


#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "esp_private/wifi.h"
#include <esp_wifi.h>

class ESP_init
{

public:
  ESP_init(int wifiChannel, wifi_phy_rate_t dataRate = WIFI_PHY_RATE_54M, uint8_t broadcastAddress[6] = nullptr) : wifiChannel_(wifiChannel), dataRate_(dataRate)
  {
    if (broadcastAddress != nullptr)
    {
      memcpy(broadcastAddress_, broadcastAddress, 6);
    }
    else
    {
      // Use the default broadcast address
      uint8_t defaultBroadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
      memcpy(broadcastAddress_, defaultBroadcastAddress, 6);
    }
  }

  uint8_t broadcastAddress_[6];

  int getWifiChannel() const;

  wifi_phy_rate_t getDataRate() const;

  const uint8_t (&getBroadcastAddress() const)[6];

  void startWifi();

  template <typename T>
  bool sendData(const T &data)
  {
    esp_err_t result = esp_now_send(getBroadcastAddress(), (uint8_t *)&data, sizeof(T));
    return result == ESP_OK;
  }


private:
  int wifiChannel_;
  wifi_phy_rate_t dataRate_;
};


  // Callback when data is received for operation mode selection
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status);



#endif // ESP_INIT_H