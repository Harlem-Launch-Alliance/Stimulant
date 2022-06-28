/*****************************************************************************
 * This file is for any settings or constants that may be manually adjusted
 * 
 ****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

//tick times (microseconds)
const unsigned int TICK_TIME_ONPAD = 10000;
const unsigned int TICK_TIME_ASCENDING = 10000;
const unsigned int TICK_TIME_DESCENDING = 50000;
const unsigned int TICK_TIME_POST_FLIGHT = 1000000;

const unsigned int RING_QUEUE_LENGTH = 3000;//each 100 elements is 1 seconds of data at 100hz

const unsigned int GPS_WAIT_TIME = 300; //number of seconds to wait for GPS to acquire signal before moving on

#endif
