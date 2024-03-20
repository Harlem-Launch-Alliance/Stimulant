/*****************************************************************************
 * This file is for all math used to determine attitude
 * 
 ****************************************************************************/

//angles are in radians
//rates are magnitudes of rad/s
//assuming roll == 0, pitchRate will be equivalent to y, and yawRate will be equivalent to x

#pragma once

#include "./utils/datatypes.h"

/// @brief this object tracks attitude/orientation as well as gyroscope calibration values
class Attitude {
public:
  Attitude() = default;
  void calibrateGyro(const Directional& rawGyro);
  void updateAttitude(Directional gyro, const bool hasLaunched, const uint32_t sampleTime);
  Directional getCurrentAttitude();
private:
  Directional m_attitude, m_zeroRateOffsets;
  uint32_t m_lastSampleTime;
  double getRate(const double roll, const double parallel, const double perpendicular);
  void getCalibratedGyro(Directional& gyro);
};
