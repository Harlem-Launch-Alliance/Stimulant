/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * not specific to any hardware architecture
 * all hardware specific settings should be in their own file
 * 
 * all hardware architectures should be conditionally included here
 ****************************************************************************/
#pragma once

#ifdef UNIT_TEST
#include "../test/lib/mock_arduino.h"
#else
#include <Arduino.h>
#endif // UNIT_TEST

#if defined(BIRDBRAIN)
#include "config/birdbrain.h"
#elif defined(CATALYST2)
#include "config/catalyst2.h"
#endif

constexpr double MAIN_ALTITUDE = 200; //altitude to deploy main parachute at (meters)
constexpr double G_FORCE_TO_LAUNCH = 2; //if acceleration exceeds this number the rocket will assume it has been launched
constexpr double MAX_APOGEE_ACCEL = 1; //we can rule out apogee if acceleration is above this amount (Gs)
