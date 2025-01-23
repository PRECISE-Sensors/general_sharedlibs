#include "Arduino.h"
#include <Wire.h>
#include "I2C_Read_general.h"
#include "BNO055_consts.h"
#include "BNO055_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <vector>
#include <iostream> // the necessary header for printing
using std::cout;
using std::endl;

BNO_config::BNO_config(uint8_t devAddr) : devAddr_(devAddr), i2cUtils_()
{
  if (!i2cUtils_.i2cAux_isDeviceAcknowledged(devAddr_))
  {
    cout << "The device doesn`t exist" << endl;
    delay(4000);
  }
  else
  {
    configured_ = true;
  };
}

/*uint8_t customAddress = 0x29; // Custom device address (replace with the desired address)
BNO_config imu(customAddress); // Will use the custom device address*/

/**
 *BNO055_OPERATION_MODE_NDOF or BNO055_OPERATION_MODE_AMG (add the parameters)*/
bool BNO_config::BNO_turn_on(uint8_t mode, uint8_t accelBandwidth, uint8_t accelRange, uint8_t gyroBandwidth, uint8_t gyroRange)
{

  bool turnON_success_ = BNO055_begin_(mode, accelBandwidth, accelRange, gyroBandwidth, gyroRange);
  delay(1000);
  return turnON_success_;
}

uint8_t BNO_config::BNO_getOperationMode() const
{
  return operation_mode_;
}
uint8_t BNO_config::getDeviceAddress() const
{
  return devAddr_;
}
bool BNO_config::isConfigured() const
{
  return configured_;
}

uint8_t BNO_config::BNO_whoami_(void)
{
  uint8_t reg_addr = BNO055_CHIP_ID_ADDR;
 // delay(1000);
cout<<"reading"<<endl;
  std::vector<uint8_t> data = i2cUtils_.i2cAux_ReadBytesRegr(devAddr_, reg_addr);
  if (!data.empty())
  {
    return data[0]; // Return the ID read from the chip
  }
  else
  {
    // Handle the case where data is empty, e.g., by returning an error code.
    // You can also throw an exception here if that fits your error handling strategy.
    return 0xFF; // Special value indicating an error.
  }
}

bool BNO_config::BNO_setMode_(uint8_t mode)
{
  uint8_t operation_mode;
  operation_mode = mode;
  // Write the mode configuration to the BNO055 sensor
  i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_OPR_MODE_ADDR, operation_mode);
  delay(30);
  return true;
}

bool BNO_config::getCalibrationStatus(uint8_t *calibData)
{
  // Read a single byte from the CALIB_STAT register
  std::vector<uint8_t> data = i2cUtils_.i2cAux_ReadBytesRegr(devAddr_, BNO055_CALIB_STAT_ADDR, 1);

  // Copy the read data into the provided calibData array
  if (!data.empty())
  {
    calibData[0] = data[0];
    return true;
  }
  else
  {
    return false; // Return false if the read data is not the expected length
  }
}

// New function to read the calibration offsets
bool BNO_config::getCalibrationOffsets(uint8_t* offsets)
{
    // Read 22 bytes of calibration data from the offset registers (0x55 to 0x6A)
    std::vector<uint8_t> data = i2cUtils_.i2cAux_ReadBytesRegr(devAddr_, 0x55, 22);

    if (data.size() == 22) 
    {
        // Copy the data into the provided array
        for (int i = 0; i < 22; i++) 
        {
            offsets[i] = data[i];
        }
        return true;
    } 
    else 
    {
        return false; // Return false if the read data is not the expected length
    }
}

bool BNO_config::setCalibrationData(const uint8_t *calibData)
{
  // The calibration data is 22 bytes long and needs to be written to the ACC_OFFSET_X_LSB register (0x55)
  uint8_t baseRegister = BNO055_ACCEL_OFFSET_X_LSB_ADDR;

  for (int i = 0; i < 22; ++i)
  {
    // Write each byte of the calibration data to the consecutive registers
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, baseRegister + i, calibData[i]);
  }

  return true; // Or return false if you have any error checking
}

bool BNO_config::BNO055_begin_(uint8_t mode, uint8_t accelBandwidth, uint8_t accelRange, uint8_t gyroBandwidth, uint8_t gyroRange)
{ // INPUT mode; setting for the frequency amd range work ONLY in AMG mode


  int timeout = 1000;            // in ms
  uint8_t operation_mode = mode; // Desired operation mode
  delay(timeout); // SHOULD BE A VERY LONG DELAY TO RESET PROPERLY!!!!

  while (BNO_whoami_() != BNO055_ID)
  {
    cout << "wrong ID..Trying again"<<endl;
    delay(timeout); // Delay for boot if the ID is incorrect
    if (BNO_whoami_() != BNO055_ID)
    {
      cout << "Device ID incorrect after delay" << endl;
      return false;
    }
     }

 cout << "Device ID is correct" << endl;
  delay(50);
 
 cout << "config  writing" << endl;
  /*Switch to config mode before making any changes*/
  BNO_setMode_(BNO055_OPERATION_MODE_CONFIG);
  delay(timeout); // SHOULD BE A VERY LONG DELAY TO RESET PROPERLY!!!!

 cout << "reset  writing" << endl;

  // /*Trigger reset and wait for sensor to boot up again*/ // At power-on Page 0 is selected
 i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_SYS_TRIGGER_ADDR, 0x20);
  delay(200); // SHOULD BE A VERY LONG DELAY TO RESET PROPERLY!!!!

 cout << "reading  ID" << endl;

  while (BNO_whoami_() != BNO055_ID)
  {
    cout << "wrong ID..Trying again"<<endl;
   // delay(20000); // Delay for boot if the ID is incorrect
    // if (BNO_whoami_() != BNO055_ID)
    // {
    //   cout << "Device ID incorrect after delay" << endl;
    //   return false;
    // }
  }
   cout << "Device ID is correct again" << endl;

    delay(timeout); // Delay for boot if the ID is incorrect

  // Set to normal power mode  // can be normal mode, low power mode, and suspend mode
  i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PWR_MODE_ADDR, BNO055_POWER_MODE_NORMAL);
    delay(timeout); // Delay for boot if the ID is incorrect

  // Register map is separated into two logical pages, Page 1 contains sensor specific configuration data
  // and Page 0 contains all other configuration parameters and output data



  // Change to register page zero
  i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PAGE_ID_ADDR, 0);
  //  i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_SYS_TRIGGER_ADDR, 0);
  delay(10);

  if (mode == BNO055_OPERATION_MODE_NDOF)
  {

    bool success = BNO_setMode_(operation_mode);
    if (!success)
    {
      cout << "Couldn`t set the mode";
    }
    else
    {
      cout << "Mode set" << endl; // debug
    }
    delay(timeout); // Delay for boot if the ID is incorrect

    // Change to register page 1, necessary to modify settings
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PAGE_ID_ADDR, 1);
    delay(timeout);

    std::vector<uint8_t> data1 = i2cUtils_.i2cAux_ReadBytesRegr(devAddr_, 0x09, 1);
    delay(timeout);

    if (!data1.empty())
    {
      uint8_t magConfig = data1[0];

      // Print the current bits of MAG_Config
      cout << "MAG_Config Register (0x09) before change: ";
      for (int i = 7; i >= 0; --i)
      {
        cout << ((magConfig >> i) & 0x01);
      }
      cout << endl;

      // Define the new value to write (binary 10011101, hex 0x9D)
      uint8_t newMagConfig = 0x9D;

      // Write the new value to the MAG_Config register
      i2cUtils_.i2cAux_WriteByteRegr(devAddr_, 0x09, newMagConfig);

      // Optionally, read back and print the new register value
      std::vector<uint8_t> newData = i2cUtils_.i2cAux_ReadBytesRegr(devAddr_, 0x09, 1);
      if (!newData.empty())
      {
        uint8_t updatedMagConfig = newData[0];
        cout << "MAG_Config Register (0x09) after change: ";
        for (int i = 7; i >= 0; --i)
        {
          cout << ((updatedMagConfig >> i) & 0x01);
        }
        cout << endl;
      }
      else
      {
        cout << "Failed to read back the MAG_Config register." << endl;
      }
   }
   // Change back to register page zero
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PAGE_ID_ADDR, 0);
    //      i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_SYS_TRIGGER_ADDR, 0);
  }

  else if (mode == BNO055_OPERATION_MODE_AMG)
  {
    // Change to register page 1, necessary to modify settings
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PAGE_ID_ADDR, 1);
    delay(20);

    // Set the accelerometer bandwitdh to 125 (250 Hz Fs) and operating range  to 8g
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_ACCEL_CONFIG, (accelBandwidth << 2) | accelRange);
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_ACCEL_CONFIG, 0x0F); // not configurable in low power mode so changing to normal?

    delay(20);
    // Set gyro bandwidth to 116 (232 Hz) and operating range to 2000 deg/s
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_GYRO_CONFIG_0, (gyroBandwidth << 3) | gyroRange);
    delay(20);

    // Change back to register page zero
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_PAGE_ID_ADDR, 0);
    i2cUtils_.i2cAux_WriteByteRegr(devAddr_, BNO055_SYS_TRIGGER_ADDR, 0);

    bool success = BNO_setMode_(operation_mode);
    if (!success)
    {
      cout << "Couldn`t set the mode";
    }
    delay(20);
  }
  operation_mode_ = operation_mode; // Store the operation mode value

  return (true);
}
