#pragma once

#include "../config/config.h"

/**
 * @brief A 3 dimensional vector
 * 
 * @note Any data that has an x,y and z attribute
 * 
 */
class Directional
{
public:
  double x,y,z; 
  Directional(double a, double b, double c){
    x = a;
    y = b;
    z = c;
  }
  Directional(){
    x = 0;
    y = 0;
    z = 0;
  }
};

/**
 * @brief Data from one altimeter sample
 * 
 */
struct bmpReading
{
  double altitude; /**< Altitude in meters*/
  uint32_t time; /**< Time of sampling in microseconds*/
  uint8_t state; /**< Phase of flight (0-3 representing ONPAD - POST_FLIGHT) */
};

/**
 * @brief All data from an IMU sample
 * 
 */
struct imuReading
{
    Directional accel; /**< Acceleration (measured in Gs)*/
    Directional gyro; /**< Rotational velocity (measured in rad/s)*/
    Directional attitude; /**< Rotational position (measured in radians)*/
    uint32_t time; /**< Time of sampling in microseconds*/
};

/**
 * @brief All data from a GPS sample
 * 
 */
struct gpsReading
{
    double latitude; /**< GPS coordinate*/
    double longitude; /**< GPS coordinate*/
    uint32_t time; /**< Time of sampling in microseconds*/
};

/**
 * @brief Phases of Flight
 * 
 * @note These phases are used to seperate the computers priorities into states
 * 
 */
enum FlightPhase {ONPAD, ASCENDING, DESCENDING, POST_FLIGHT};
