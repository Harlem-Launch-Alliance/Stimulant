/*****************************************************************************
 * This file is for declaring utilities to be used in multiple other places
 * 
 ****************************************************************************/
#pragma once

#include "datatypes.h"

/**
 * @brief Convert radians to degrees
 * 
 * @param angle Angle in radians
 * @return double Angle in degrees
 */
double toDeg(double angle);

/**
 * @brief Convert degrees to radians
 * 
 * @param angle Angle in degrees
 * @return double Angle in radians
 */
double toRad(double angle);

/**
 * @brief Get the tick time based on the current flight phase
 * 
 * @param phase Flight phase
 * @return int Tick length in microseconds
 */
int getTickTime(flightPhase phase);
