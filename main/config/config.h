/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * not specific to any hardware architecture
 * all hardware specific settings should be in their own file
 * 
 * all hardware architectures should be conditionally included here
 ****************************************************************************/
#pragma once

#include <Arduino.h>

#if defined(BIRDBRAIN)
#include "config/birdbrain.h"
#elif defined(CATALYST2)
#include "config/catalyst2.h"
#endif

constexpr double MAIN_ALTITUDE = 300; //altitude to deploy main parachute at (meters)
