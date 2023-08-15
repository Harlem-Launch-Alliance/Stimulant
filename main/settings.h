/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * 
 ****************************************************************************/
#pragma once

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include "../test/lib/mock_arduino.h"
#endif // UNIT_TEST

//tick times (microseconds)
constexpr uint32_t TICK_TIME_ONPAD = 10000;
constexpr uint32_t TICK_TIME_ASCENDING = 10000;
constexpr uint32_t TICK_TIME_DESCENDING = 50000;
constexpr uint32_t TICK_TIME_POST_FLIGHT = 1000000;

constexpr uint32_t GPS_WAIT_TIME = 300; //number of seconds to wait for GPS to acquire signal before moving on

constexpr int BMP_CS = 10; //SPI chipSelect of bmp altimeter
#ifdef PICO
#define GPSSerial Serial1 //Hardware Serial Location of GPS
#define XBeeSerial Serial //Hardware Serial Location of XBee
constexpr uint32_t RING_QUEUE_LENGTH = 1000;//each 100 elements is 1 seconds of data at 100hz
#else
#define XBeeSerial Serial1 //Hardware Serial Location of XBee
#define GPSSerial Serial2 //Hardware Serial Location of GPS
constexpr uint32_t RING_QUEUE_LENGTH = 3000;//each 100 elements is 1 seconds of data at 100hz
#endif // PICO
constexpr int BUZZER_PIN = 33;