/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * specific to the Birdbrain flight computer
 ****************************************************************************/
#pragma once

//tick times (microseconds)
constexpr uint32_t TICK_TIME_ONPAD = 10000;
constexpr uint32_t TICK_TIME_ASCENDING = 10000;
constexpr uint32_t TICK_TIME_DESCENDING = 50000;
constexpr uint32_t TICK_TIME_POST_FLIGHT = 1000000;

constexpr uint32_t GPS_WAIT_TIME = 300; //number of seconds to wait for GPS to acquire signal before moving on

constexpr uint32_t RING_QUEUE_LENGTH = 1000;//each 100 elements is 1 seconds of data at 100hz

// pin definitions
constexpr int BMP_CS = 10; //SPI chipSelect of bmp altimeter
constexpr int BUILTIN_SDCARD = 12; // SPI chipSelect of SD card
constexpr int RADIO_CS = 11; //SPI chipSelect of radio module
constexpr int IMU_CS = 9; //SPI chipSelect of IMU
constexpr int SPI0_SCK = 4; //SPI clock pin
constexpr int SPI0_MOSI = 5; //SPI MOSI pin
constexpr int SPI0_MISO = 6; //SPI MISO pin
constexpr int SPI1_SCK = 14;
constexpr int SPI1_MOSI = 15;
constexpr int SPI1_MISO = 16;
constexpr int BUZZER_PIN = 33;

#define GPSSerial Serial1 //Hardware Serial Location of GPS
#define XBeeSerial Serial //Hardware Serial Location of XBee
