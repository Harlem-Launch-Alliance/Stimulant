/*****************************************************************************
 * This file is for all imu queries for the a mock IMU
 ****************************************************************************/

#ifdef UNIT_TEST

#include "imu.h"
#include "../../utils/utils.h"

imuReading imu::sample()
{
  imuReading imuSample;
  imuSample.time = 5; //current timestamp

  imuSample.accel.x = 1
  imuSample.accel.y = 2;
  imuSample.accel.z = 3;
  imuSample.gyro.x = 4;
  imuSample.gyro.y = 5;
  imuSample.gyro.z = 6; //IMU's Y corresponds to rocket's Z axis due to orientation on PCB

  return imuSample; //return gyro and accel data
}

void imu::setup() {}

#endif // UNIT_TEST
