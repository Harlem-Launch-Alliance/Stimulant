/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/
#pragma once

#include "config/config.h"
#include "utils/datatypes.h"
#include "utils/ringQueue.h"

/**
 * @brief Transmit telemtry to ground station via radio
 * 
 * @param altitude Current altitude
 * @param gps Current GPS location
 * @param phase Current phase of flight (On pad, ascending, etc.)
 */
void transmitData(double altitude, gpsReading gps, char phase);

class SDCard {
public:
  SDCard();
  ~SDCard() = default;

  /**
 * @brief Establish connection to SD card
 * 
 * Attempt to write data to SD card to verify connection
 * 
 * @param date Current date/time
 */
  void setup(String date);

  /**
   * @defgroup cacheData Cache Data
   * 
   * @note If the rocket has not yet launched, all data that is more than 2 seconds old will be trimmed to ensure that there is space in the cache for flight data.
   * 
   * @param sample Data sample to be cached
   * @param prelaunch Current status of rocket (launched or not)
   * @{
   */
  void record(imuReading sample, bool prelaunch);

  void record(bmpReading sample, bool prelaunch);

  void record(gpsReading sample, bool prelaunch);
  /**@}*/

  /**
   * @brief write all data from cache to SD card
   * 
   */
  void backup();

private:
  char filename[50];
  RingQueue<imuReading, RING_QUEUE_LENGTH_IMU> imuQueue;
  RingQueue<bmpReading, RING_QUEUE_LENGTH_ALTIMETER> bmpQueue;
  RingQueue<gpsReading, RING_QUEUE_LENGTH_GPS> gpsQueue;

  mutex_t m_mutex;
};
