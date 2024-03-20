/*****************************************************************************************
 * 
 * Interface for main functions to be used either in a single process or multiprocess
 * 
*****************************************************************************************/

#pragma once

#include "calculations/attitude.h"
#include "sensors/imu/imu.h"
#include "utils/datatypes.h"


class RocketController {
    public:
        RocketController();
        ~RocketController() = default;

        /// @brief setup sensors and other hardware
        void setup();

        /// @brief setup second core
        void setup1();

        /// @brief main loop for the main core
        void loop();

        /// @brief main loop for the second core
        void loop1();
    private:
        /// @brief run the on pad phase
        FlightPhase runOnPad();

        /// @brief run the ascending phase
        FlightPhase runAscending();

        /// @brief run the descending phase
        FlightPhase runDescending();

        /// @brief run the post flight phase
        FlightPhase runPostFlight();

        /// @brief initialize pyro channels
        void setupPyros();

        volatile FlightPhase state;
        uint32_t tick;

        IMU imu;

        // current attitude (orientation) of the rocket
        Attitude attitude;
};
