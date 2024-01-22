/*************************************************
* This is an interface for all radio functions
*
*************************************************/

#pragma once

#include <Arduino.h>

class Radio
{
    public:
    /**
     * @brief Send a message to the ground station
     * 
     * @param message 
     */
    void print(String message);

    /**
     * @brief Send a message to the with an endline to the ground station
     * 
     * @param message 
     */
    void println(String message);

    /**
     * @brief Receive a message from the ground station
     * 
     * @return String 
     */
    String receive();

    /**
     * @brief Setup the radio
     * 
     */
    void setup();
};