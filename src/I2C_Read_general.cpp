#ifndef EXCLUDE_I2C_READ_GENERAL

#include <stdbool.h>
#include <stdint.h>
#include <Wire.h>
#include "Arduino.h"
#include "I2C_Read_general.h"
#include <stdbool.h>
#include <stdint.h>
#include <vector>
#include <iostream> // Include the necessary header for printing
using std::cout;
using std::endl;



  std::vector<uint8_t> ReadI2C_general::i2cMain_ReadBytes(uint8_t address, uint8_t count)
  {
    std::vector<uint8_t> data;
    Wire.requestFrom(address, count); // Read bytes from slave /register/ address
    while (Wire.available() && data.size() < count)
    {
      data.push_back(Wire.read());
    }
    return data;
  }

  std::vector<uint8_t> ReadI2C_general::i2cAux_ReadBytesRegr(uint8_t address, uint8_t subAddress, uint8_t count )
  {

  
    std::vector<uint8_t> data;
    Wire1.beginTransmission(address);  // Initialize the Tx buffer
    //delay(1500); // Add a short delay (10 ms or adjust as needed)

    Wire1.write(subAddress);           // Put slave register address in Tx buffer
    Wire1.endTransmission(false);      // Send the Tx buffer, but send a restart to keep connection alive
   // delay(600); // Add a short delay (10 ms or adjust as needed)

    int bytesReceived = Wire1.requestFrom(address, count); // Read bytes from slave register address


   if (Wire1.available()) {
        while (data.size() < count) {
            data.push_back(Wire1.read());
        }
    }else {
       cout << "I2C communication error: expected " << count << " bytes, but received " << bytesReceived << endl;
    return data; // or handle the error as needed
   }

    return data;
  }

  void ReadI2C_general::i2cMain_WriteByteRegr(uint8_t dev_addr, uint8_t data)
  {
    Wire.beginTransmission(dev_addr);

    /*if (writeReg)
    {
      Wire.write(reg_addr);
    }*/

    Wire.write(data);

    if (Wire.endTransmission(true) == 0)
    {
      cout << "Transmission successful" << endl;
    }
    else
    {
      cout << "Transmission unsuccessful" << endl;
    }
  }

  void ReadI2C_general::i2cAux_WriteByteRegr(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
  {
    Wire1.beginTransmission(dev_addr);
    Wire1.write(reg_addr);

    if (Wire1.write(data) == 1)
    {
      //  cout << "Written correctly" << endl;
    }

    if (Wire1.endTransmission(true) == 0)
    {
      //    cout << "Successful transmission" << endl;
    }
    else
    {
      cout << "Unsuccessful transmission" << endl;
    }
  }

  bool ReadI2C_general::i2cAux_isDeviceAcknowledged(uint8_t address)
  {
    Wire1.beginTransmission(address);
    int result = Wire1.endTransmission();
    return result == 0;
  }

  uint8_t ReadI2C_general::calculateChecksum8(const std::vector<uint8_t> &data)
  {
    uint8_t sum = 0;
    size_t dataSize = data.size();
    for (size_t i = 0; i < dataSize; i++)
    {
      sum += data[i];
    }
    return sum;
  }

  int16_t ReadI2C_general::combineInt8ToInt16(uint8_t lowByte, uint8_t highByte)
  {
    int16_t result = lowByte | (highByte << 8);
    return result;
  }

  uint32_t ReadI2C_general::combineInt8ToUInt32(const uint8_t *byte1, const uint8_t *byte2, const uint8_t *byte3, const uint8_t *byte4)
  {
    uint32_t result = 0;
    result |= static_cast<uint32_t>(*byte1) << 24; // Combine the most significant byte
    result |= static_cast<uint32_t>(*byte2) << 16; // Combine the second most significant byte
    result |= static_cast<uint32_t>(*byte3) << 8;  // Combine the third most significant byte
    result |= static_cast<uint32_t>(*byte4);       // Combine the least significant byte
    return result;
  }

  // Function to divide int16_t value into two uint8_t values
  void ReadI2C_general::divideInt16ToInt8(int16_t value, uint8_t *lowByte, uint8_t *highByte)
  {
    *lowByte = value & 0xFF;         // Extract the low byte
    *highByte = (value >> 8) & 0xFF; // Extract the high byte
  }

  // Function to divide uint32_t value into two uint8_t values

  void ReadI2C_general::divideUInt32ToInt8(uint32_t value, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3, uint8_t *byte4)
  {
    *byte1 = (value >> 24) & 0xFF; // Extract the most significant byte
    *byte2 = (value >> 16) & 0xFF; // Extract the second most significant byte
    *byte3 = (value >> 8) & 0xFF;  // Extract the third most significant byte
    *byte4 = value & 0xFF;         // Extract the least significant byte
  }

#endif
