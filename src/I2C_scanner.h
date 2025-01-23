

// Include Arduino Wire library for I2C
#include <Arduino.h>
#include <vector>
#include <stdbool.h>
#include <stdint.h>

 
// Define Slave I2C Address
// Define Slave answer size
#define ANSWERSIZE 1

/*
extern byte imu_addresses[20];
extern byte emg_addresses[20];
extern int num_imu_slaves;
extern int num_emg_slaves;
extern   int imu_counts_wire;*/
std::vector<byte> findSlaves_IMU();

