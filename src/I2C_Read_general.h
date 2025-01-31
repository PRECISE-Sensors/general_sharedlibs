#ifndef EXCLUDE_I2C_READ_GENERAL

#ifndef I2C_FUNCTIONS_H
#define I2C_FUNCTIONS_H

#include "Commons.h"
#include <stdbool.h>
#include <vector>
#include <iostream> // Include the necessary header for printing and templates 
using std::cout;


class ReadI2C_general
{

  // MAIN bus - uses Wire  //AUX bus - uses Wire1

public:
  ReadI2C_general()
  {
  }

  std::vector<uint8_t> i2cMain_ReadBytes(uint8_t address, uint8_t count = 1);
 

  std::vector<uint8_t> i2cAux_ReadBytesRegr(uint8_t address, uint8_t subAddress, uint8_t count = 1);


  void i2cMain_WriteByteRegr(uint8_t dev_addr, uint8_t data);
 

  void i2cAux_WriteByteRegr(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
 
  bool i2cAux_isDeviceAcknowledged(uint8_t address);
 
  uint8_t calculateChecksum8(const std::vector<uint8_t> &data);
  
  int16_t combineInt8ToInt16(uint8_t lowByte, uint8_t highByte);
  

  uint32_t combineInt8ToUInt32(const uint8_t *byte1, const uint8_t *byte2, const uint8_t *byte3, const uint8_t *byte4);
 

  // Function to divide int16_t value into two uint8_t values
  void divideInt16ToInt8(int16_t value, uint8_t *lowByte, uint8_t *highByte);
  
  // Function to divide uint32_t value into two uint8_t values

  void divideUInt32ToInt8(uint32_t value, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3, uint8_t *byte4);
  
  template <size_t N>
  void divideInt16ToInt8_Loop(const std::array<int16_t, N> &values, std::vector<uint8_t> &outputArray)
  {
    size_t size = values.size();
    outputArray.resize(size * 2); // Resize the output array, twice the size of the original as we divide 1int16 into int8

    for (size_t i = 0; i < size; ++i)
    {
      divideInt16ToInt8(values[i], &outputArray[i * 2], &outputArray[i * 2 + 1]);
    }
  }
};

#endif // I2C_FUNCTIONS_H

