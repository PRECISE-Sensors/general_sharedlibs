

#include <Arduino.h>
#include "I2C_scanner.h"
#include <Wire.h> // For I2C communication with P5LP
#include <vector>
#include <chrono>
#include <iostream>
#include <vector>
#include <array>
using std::cout;
using std::endl;

// CURRENTLY NUMBER OF SCAN IS 3
// WILL REPEAT 3 TIMES FOR EACH BUS, AND IF ANY OF THESE BUSES GIVES INCONSISTENT RESULT - WIll REPEAT

/* --------------CHANGE THE ARRAYS TO VECTORS FOR AUTOMATIC SIZE ESTIMATION--------------------------

void findSlaves(std::vector<byte>& imu_addresses, std::vector<byte>& emg_addresses) {


//every time you find emg address
  emg_addresses.push_back(emg_address);

  imu_addresses.push_back(imu_address);

}*/

/*byte imu_addresses[20]; //arbitrary size
byte emg_addresses[20];
int num_imu_slaves = 0;
int num_emg_slaves = 0;*/

// findSlaves(byte imu_addresses[], byte emg_addresses[], int& num_imu_slaves, int& num_emg_slaves)

 std::vector<byte> findSlaves_IMU()
{

  const int8_t numOfScans = 1;

  std::vector<byte> imuAddrs_aux;

  byte error, address;
  bool imu_consistent = true;
  std::vector<int> imu_counts_wire1;

  while (1)
  {

   for (int k = 0; k < numOfScans; k++)
    {
      cout << "Scanning bus Wire1 (AUX)..." << endl;

      imuAddrs_aux.clear();

      for (address = 1; address < 127; address++)
      {
        // The i2c_scanner uses the return value of the Write.endTransmisstion to see if a device did acknowledge to the address.
        Wire1.beginTransmission(address);
        error = Wire1.endTransmission();

        if (error == 0)
        {
          cout << "Wire1 I2C device found at address 0x";
          if (address < 16)
            cout << "0";
          cout << std::hex << static_cast<int>(address);
          cout << "  !" << endl;
          if (address < 20)
          {

            cout << "Wrong device"
                 << "\n";
          }
          else
          {
            // Serial.print(imu_counts[0]);
              imuAddrs_aux.push_back(address);
          }
        }
        else if (error == 4)
        {
          cout << "Unknown error at address 0x";
          if (address < 16)
            cout << "0";
          cout << std::hex << static_cast<int>(address);
          cout << "  !" << endl;
        }
      }

      imu_counts_wire1.push_back(imuAddrs_aux.size());
    //  cout << imu_counts_wire1[k] << endl;
   
      delay(3000); // wait 3 seconds for next scan
    }


    
    //IF WE WANT ALL IN ONE ARRAY
   // imuAddrs.insert(imuAddrs.end(), imuAddrs_aux.begin(), imuAddrs_aux.end());
    // Check consistency of imu_count and emg_count for each scan

    for (int i = 1; i < numOfScans; i++)

    {
     /*cout<<"IMU counts main: ";
       cout<<imu_counts[0]<<endl;
       cout<<imu_counts[2]<<endl;
       cout<<"IMU counts aux: ";
       cout<<imu_counts_wire1[0]<<endl;
       cout<<imu_counts_wire1[2]<<endl;
       cout<<"EMG counts main: ";
       cout<<emg_counts[0]<<endl;
       cout<<emg_counts[2]<<endl;*/

      if ( imu_counts_wire1[i] != imu_counts_wire1[0])
      {
        imu_consistent = false;
      }
    }

    if (imu_consistent)
    {

      cout << "Consistent number of devices found for IMU arrays." << endl;
      cout << "Number of IMUs found: " << imuAddrs_aux.size()<< endl;

      break;
    }

    // Print error message if counts are inconsistent
    if (!imu_consistent)
    {
      
      cout << "Error: Inconsistent number of IMU devices found." << endl;
    }
    
  }

  //cout << "Task Find slaves finished on core ";
 // cout << xPortGetCoreID() << endl;

  //cout << "Find slaves: priority = ";
 // cout << uxTaskPriorityGet(NULL) << endl;

  
  // Check if emgAddrs and imuAddrs are empty
  if ( imuAddrs_aux.empty())
  {
    // Return an empty pair indicating no devices were found
    cout << "No devices found." << endl;
    return {{}};
  }
  else
  {
    // Return the vectors as a pair (or a tuplr) if devices were found
    return { imuAddrs_aux};
  }
}
