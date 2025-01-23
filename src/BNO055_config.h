
#ifndef BNO055_CONFIG_H
#define BNO055_CONFIG_H

#include <Arduino.h>
#include "BNO055_consts.h"
#include "I2C_Read_general.h"




class BNO_config
{

public:
  /**
   *Can be either BNO055_ADDRESS_A (default, 0x28) or BNO055_ADDRESS_B (0x29)*/
  BNO_config(uint8_t devAddr = BNO055_ADDRESS_A) ;
  /**
   *BNO055_OPERATION_MODE_NDOF or BNO055_OPERATION_MODE_AMG (add the parameters)*/
  bool BNO_turn_on(uint8_t mode = BNO055_OPERATION_MODE_NDOF, uint8_t accelBandwidth = BNO055_ACCEL_BW_125HZ, uint8_t accelRange = BNO055_ACCEL_RANGE_8G, uint8_t gyroBandwidth = BNO055_GYRO_BW_116HZ, uint8_t gyroRange = BNO055_GYRO_RANGE_2000DPS);

  uint8_t BNO_getOperationMode() const;
 
  uint8_t getDeviceAddress() const;
 
  bool isConfigured() const;
  bool getCalibrationStatus(uint8_t *calibData);
  bool setCalibrationData(const uint8_t *calibData);
  bool getCalibrationOffsets(uint8_t* offsets);
    bool BNO_setMode_(uint8_t mode);


private:
  uint8_t devAddr_;
  ReadI2C_general i2cUtils_;
  bool configured_ = false; // Flag to track configuration status
  uint8_t operation_mode_;

  uint8_t BNO_whoami_(void);



  
 

  bool BNO055_begin_(uint8_t mode, uint8_t accelBandwidth, uint8_t accelRange, uint8_t gyroBandwidth, uint8_t gyroRange);
 
};
#endif // BNO055_CONFIG_H
