#ifndef BNO_READI2C_H
#define BNO_READI2C_H

#include <Arduino.h>
#include "I2C_Read_general.h"
#include "BNO055_config.h"
#include "Commons.h"




class BNO_ReadI2C
{

public:
  BNO_ReadI2C(BNO_config &config) ;
  std::vector<ImuData_raw> BNO_Read_raw() const;

  std::vector<ImuData_fl> BNO_Read_fl();
 

  void BNO_getAMG();
 

  void BNO_getAcc();


  void BNO_getGyr();


  void BNO_getMag();
 

  void BNO_getEuler();


  void BNO_getQuat();
 
private:
  BNO_config &config_;
  uint8_t devAddr_; // 0x028 by default
  uint8_t operation_mode_;
  bool turnON_success_;

  ReadI2C_general i2cUtils;
  std::vector<ImuData_raw> imuData_;
  std::vector<ImuData_fl> imuDatafl_;

  void BNO_convertToFloat();
 
  ImuData_raw BNO_readIMU_TypeAddrs(ImuDataType dataType, uint8_t addrs);
  
};





#endif // BNO_READI2C_H
