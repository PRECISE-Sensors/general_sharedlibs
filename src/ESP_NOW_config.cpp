#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "esp_private/wifi.h"
#include <esp_wifi.h>
#include <ESP_NOW_config.h>
#include <iostream> // the necessary header for printing

using std::cout;
using std::endl;

  int ESP_init::getWifiChannel() const
  {
    return wifiChannel_;
  }

  wifi_phy_rate_t ESP_init::getDataRate() const
  {
    return dataRate_;
  }

  const uint8_t (&ESP_init::getBroadcastAddress() const)[6]
  {
    return broadcastAddress_;
  }
   
   
    esp_now_peer_info_t peerInfo; // Stores information about the peer accordign to the ESP NOW protocol.



  void ESP_init::startWifi()
  {


    cout<<"i am starting esp init"<<endl;

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);

    esp_wifi_stop();
    esp_wifi_deinit();

    esp_netif_init();
    esp_event_loop_create_default();
    /*Disabling AMPDU is necessary to set a fix rate*/
    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT(); // We use the default config ...
    my_config.ampdu_tx_enable = 0;                             //... and modify only what we want.
    esp_wifi_init(&my_config);                                 // set the new config
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_STA);

    esp_wifi_start();

    esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT20);                                             // bandwidth
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N); // protocol
    esp_wifi_set_channel(wifiChannel_, WIFI_SECOND_CHAN_NONE);                                     // might need better routine for keeping track on different channels, to not to lose the connection

    esp_wifi_config_espnow_rate(WIFI_IF_STA, dataRate_); /*set the rate*/


    // Init ESP-now
    if (esp_now_init() != ESP_OK)
    {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress_, 6);
    // peerInfo.channel = 0;
    peerInfo.ifidx = WIFI_IF_STA;
    peerInfo.encrypt = false;
    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
      Serial.println("Failed to add peer");
      return;
    }

    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSend);
    cout << "esp_init done" << endl;
  }
  
