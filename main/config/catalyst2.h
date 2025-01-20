/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * specific to the Catalyst-2 flight computer
 ****************************************************************************/
#pragma once

//tick times (microseconds)
constexpr uint32_t TICK_TIME_ONPAD = 10000;
constexpr uint32_t TICK_TIME_ASCENDING = 10000;
constexpr uint32_t TICK_TIME_DESCENDING = 50000;
constexpr uint32_t TICK_TIME_POST_FLIGHT = 1000000;

constexpr uint32_t GPS_WAIT_TIME = 300; //number of seconds to wait for GPS to acquire signal before moving on

constexpr uint32_t RING_QUEUE_LENGTH = 3000;//each 100 elements is 1 second of data at 100hz

// pin definitions
constexpr int BMP_CS = 10; //SPI chipSelect of bmp altimeter
constexpr int SPI0_SCK = 11; //SPI clock pin (used for built in SD card)
constexpr int SPI0_MOSI = 12; //SPI MOSI pin
constexpr int SPI0_MISO = 13; //SPI MISO pin
constexpr int SPI1_SCK = SPI0_SCK;
constexpr int SPI1_MOSI = SPI0_MOSI;
constexpr int SPI1_MISO = SPI0_MISO;
constexpr int BUZZER_PIN = 33;
constexpr int PYRO0_PIN = 34;
constexpr int PYRO1_PIN = 35;

constexpr int TONE_HZ = 1000; //frequency of buzzer tone

#define XBeeSerial Serial1 //Hardware Serial Location of XBee
#define GPSSerial Serial2 //Hardware Serial Location of GPS
