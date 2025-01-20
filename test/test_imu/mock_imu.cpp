/*****************************************************************************
 * This file is for all imu queries for the a mock IMU
 ****************************************************************************/

#include "../../main/sensors/imu/imu.h"

imuReading IMU::sample()
{
  imuReading imuSample;
  imuSample.time = 5; //current timestamp

  imuSample.accel.x = 1;
  imuSample.accel.y = 2;
  imuSample.accel.z = 3;
  imuSample.gyro.x = 4;
  imuSample.gyro.y = 5;
  imuSample.gyro.z = 6;

  return imuSample; //return gyro and accel data
}

void IMU::setup() {}
