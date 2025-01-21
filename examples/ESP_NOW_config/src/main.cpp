#include <ESP_NOW_config.h>

/* Example for the transmitter using ESP-NOW 
 * - Choose the appropriate data rate and channel for communication.
 * - Refer to the ESP-IDF documentation for supported data rates: 
 *   https://github.com/espressif/esp-idf/blob/625bd5eb1806809ff3cc010ee20d1f750aa778a1/components/esp_wifi/include/esp_wifi_types.h#L474
 * - Ensure the channel number, data rate, and packet structure match on all communicating devices.
 * - By default, broadcast communication is used. Peer-to-peer communication can be configured by specifying a MAC address.
 */

// Define the structure of your ESP-NOW data packet
struct ESPNow_pckt {
    int sensorID;
    float temperature;
    float humidity;
};

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){};

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status){};


void setup() {
    Serial.begin(921600); // Initialize serial communication

    // Example receiver MAC address
    uint8_t rx_address[6] = {0xD4, 0xD4, 0xDA, 0x0F, 0xDB, 0xE4};
    
    // Create ESP-NOW object with WiFi channel 11 and a specific data rate
    ESP_init espObject(11, WIFI_PHY_RATE_54M, rx_address);

    // Initialize ESP-NOW and WiFi
    espObject.startWifi();
}

void loop() {
    // Create an ESP-NOW packet with random values
    ESPNow_pckt res_all;
    res_all.sensorID = 1; // Example sensor ID
    res_all.temperature = random(20, 30); // Random temperature between 20 and 30
    res_all.humidity = random(40, 60);     // Random humidity between 40% and 60%

    // Print the values being sent
    Serial.print("Sending Packet: Sensor ID: ");
    Serial.print(res_all.sensorID);
    Serial.print(", Temperature: ");
    Serial.print(res_all.temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(res_all.humidity);
    Serial.println(" %");

    // Send the packet and handle result
    if (!espObject.sendData(res_all)) {
        Serial.println("Failed to send packet.");
    } else {
        Serial.println("Packet sent successfully.");
    }

    delay(1000); // Wait for 1 second before sending the next packet
}
