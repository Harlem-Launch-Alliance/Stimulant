/**********************************************
* This is an interface for all IMU functions
*
**********************************************/

#pragma once

#include "../../utils/datatypes.h"

// Interface for IMU functions
class imu
{
    public:
    /**
     * @brief Sample the IMU (Gyroscope and Accelerometer)
     * 
     * @return imuReading 
     */
    imuReading sample();

    /**
     * @brief setup the IMU
     * 
     */
    void setup();
};
