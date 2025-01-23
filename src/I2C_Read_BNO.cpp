#include <Arduino.h>
#include "I2C_Read_general.h"
#include "Commons.h"
#include "BNO055_consts.h"
#include "BNO055_config.h"
#include "I2C_Read_BNO.h"
#include <stdbool.h>
#include <stdint.h>
#include <vector>
#include <iostream> // Include the necessary header for printing
using std::cout;
using std::endl;



BNO_ReadI2C::BNO_ReadI2C(BNO_config &config) : config_(config), devAddr_(config.getDeviceAddress()), i2cUtils()
  {
    if (!config_.isConfigured())
    {
      cout << "BNO_config is not properly configured." << endl;
      delay(5000);
    }
  }

  std::vector<ImuData_raw> BNO_ReadI2C::BNO_Read_raw() const
  {
    return imuData_;
  }

  std::vector<ImuData_fl> BNO_ReadI2C::BNO_Read_fl()
  {
    BNO_convertToFloat();
    return imuDatafl_;
  }

  void BNO_ReadI2C::BNO_getAMG()
  {
    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::AMG, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }

  void BNO_ReadI2C::BNO_getAcc()
  {
    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::Acc, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }

  void BNO_ReadI2C::BNO_getGyr()
  {
    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::Gyr, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }

  void BNO_ReadI2C::BNO_getMag()
  {
    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::Mag, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }

  void BNO_ReadI2C::BNO_getEuler()
  {
    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::Euler, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }

  void BNO_ReadI2C::BNO_getQuat()
  {

    std::vector<ImuData_raw> imu_vals;
    ImuData_raw dataReading = BNO_readIMU_TypeAddrs(ImuDataType::Quat, devAddr_);
    imu_vals.push_back(dataReading);
    imuData_ = imu_vals;
  }


  void BNO_ReadI2C::BNO_convertToFloat()
  {
    std::vector<ImuData_fl> imu_vals_float;
    for (const auto &imu_data_raw : imuData_)
    {
      ImuData_fl data_fl;
      for (size_t i = 0; i < 3; ++i)
      {
        data_fl.acc[i] = static_cast<float>(imu_data_raw.acc[i]) / 100.0f;    // 1 m/s^2 = 100 LSB (raw data)
        data_fl.gyr[i] = static_cast<float>(imu_data_raw.gyr[i]) / 16.0f;     // 1 deg/s = 16 LSB (raw data)
        data_fl.mag[i] = static_cast<float>(imu_data_raw.mag[i]) / 16.0f;     // 1 uT = 16 LSB (raw data)
        data_fl.euler[i] = static_cast<float>(imu_data_raw.euler[i]) / 16.0f; // 1 degree = 16 LSB (raw data)
      }
      for (size_t i = 0; i < 4; ++i)
      {
        data_fl.quat[i] = static_cast<float>(imu_data_raw.quat[i]) / static_cast<float>(1 << 14) * 1.0f; // 1 Quaternion (unitless) = 2^14 LSB (raw data)
      }
      imu_vals_float.push_back(data_fl);
    }
    imuDatafl_ = imu_vals_float;
  }

  ImuData_raw BNO_ReadI2C::BNO_readIMU_TypeAddrs(ImuDataType dataType, uint8_t addrs)
  {
    ImuData_raw dataRead;
    std::vector<uint8_t> data;
    if (dataType == Acc || dataType == Gyr || dataType == Mag || dataType == AMG)
    {

      data = i2cUtils.i2cAux_ReadBytesRegr(addrs, BNO055_ACCEL_DATA_X_LSB_ADDR, 18);

      // Print the address and subaddress for debugging
      /*std::cout << "Read from address: 0x" << std::hex << static_cast<int>(addrs)
                << ", Subaddress: 0x" << static_cast<int>(BNO055_ACCEL_DATA_X_LSB_ADDR)
                << std::dec << std::endl;*/
      // Print the content of the data vector for debugging
      // std::cout << "Data read from sensor:";
      // for (uint8_t i = 0; i < data.size(); i++) {
      //  std::cout << " " << static_cast<int>(data[i]);
      //}
      // std::cout << std::endl;

      for (uint8_t i = 0; i < 3; i++)
      {
        if (dataType == Acc || dataType == AMG)
        {
          dataRead.acc[i] = i2cUtils.combineInt8ToInt16(data[i * 2], data[i * 2 + 1]);
          //    std::cout << "Combined Acc: " << static_cast<int>(dataRead.acc[i]) << ", "; // debugging
        }
        if (dataType == Gyr || dataType == AMG)
        {
          dataRead.gyr[i] = i2cUtils.combineInt8ToInt16(data[i * 2 + 12], data[i * 2 + 13]);
         //               cout << dataRead.gyr[i] << " ,"; // Print combined Mag data

        }
        if (dataType == Mag || dataType == AMG)
        {
          dataRead.mag[i] = i2cUtils.combineInt8ToInt16(data[i * 2 + 6], data[i * 2 + 7]);
          //      cout << dataRead.mag[i] << " "; // Print combined Mag data

        }
        //cout<<endl;
      }

      //  std::cout << std::endl;
    }
    else if (dataType == Euler)
    {
      data = i2cUtils.i2cAux_ReadBytesRegr(addrs, BNO055_EULER_H_LSB_ADDR, 6);
      for (uint8_t i = 0; i < 3; i++)
      {
        dataRead.euler[i] = i2cUtils.combineInt8ToInt16(data[i * 2], data[i * 2 + 1]);
      }
    }
    else if (dataType == Quat)
    {
      data = i2cUtils.i2cAux_ReadBytesRegr(addrs, BNO055_QUATERNION_DATA_W_LSB_ADDR, 8);
      for (uint8_t i = 0; i < 4; i++)
      {
        dataRead.quat[i] = i2cUtils.combineInt8ToInt16(data[i * 2], data[i * 2 + 1]);
      }
    }

    return dataRead;
  }
