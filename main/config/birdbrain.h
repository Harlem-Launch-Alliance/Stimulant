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

constexpr uint32_t GPS_WAIT_TIME = 900; //number of seconds to wait for GPS to acquire signal before moving on

constexpr uint32_t RING_QUEUE_LENGTH = 1000;//each 100 elements is 1 seconds of data at 100hz

// pin definitions
constexpr int BMP_CS = 7; //SPI chipSelect of bmp altimeter
constexpr int BUILTIN_SDCARD = 9; // SPI chipSelect of SD card
constexpr int RADIO_CS = 8; //SPI chipSelect of radio module
constexpr int IMU_CS = 6; //SPI chipSelect of IMU
constexpr int SPI0_SCK = 2; //SPI clock pin
constexpr int SPI0_MOSI = 3; //SPI MOSI pin
constexpr int SPI0_MISO = 4; //SPI MISO pin
constexpr int SPI1_SCK = 10;
constexpr int SPI1_MOSI = 11;
constexpr int SPI1_MISO = 12;
constexpr int BUZZER_PIN = 26;
constexpr int PYRO0_PIN = 14;
constexpr int PYRO1_PIN = 15;

constexpr int TONE_HZ = 2500; //frequency of buzzer tone

static constexpr double GPSSerial Serial1 //Hardware Serial Location of GPS
static constexpr double XBeeSerial Serial //Hardware Serial Location of XBee
