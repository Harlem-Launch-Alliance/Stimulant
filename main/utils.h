/*****************************************************************************
 * This file is for declaring utilities to be used in multiple other places
 * 
 ****************************************************************************/
#include "settings.h"
#include <Arduino.h>

#ifndef UTILS_H
#define UTILS_H

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
    Directional gyro; /**< Rotational velocity (measured in degrees/s)*/
    Directional attitude; /**< Rotational position (measured in degrees)*/
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
 * @brief Convert radians to degrees
 * 
 * @param angle Angle in radians
 * @return double Angle in degrees
 */
double toDeg(double angle){
  return ((angle) *(180/PI));
}

/**
 * @brief Convert degrees to radians
 * 
 * @param angle Angle in degrees
 * @return double Angle in radians
 */
double toRad(double angle){
  return ((angle) * (PI/180));
}

/**
 * @brief Phases of Flight
 * 
 * @note These phases are used to seperate the computers priorities into states
 * 
 */
enum flightPhase {ONPAD, ASCENDING, DESCENDING, POST_FLIGHT};

/**
 * @brief Get the tick time based on the current flight phase
 * 
 * @param phase Flight phase
 * @return int Tick length in microseconds
 */
int getTickTime(flightPhase phase){//map flight phases to tick times
  switch(phase){
  case ONPAD:
    return TICK_TIME_ONPAD;
  case ASCENDING:
    return TICK_TIME_ASCENDING;
  case DESCENDING:
    return TICK_TIME_DESCENDING;
  case POST_FLIGHT:
    return TICK_TIME_POST_FLIGHT;
  }
  return TICK_TIME_POST_FLIGHT;
}

#endif