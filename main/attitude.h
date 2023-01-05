/*****************************************************************************
 * This file is for all math used to determine attitude
 * 
 ****************************************************************************/

//angles are in radians
//rates are magnitudes of rad/s
//assuming roll == 0, pitchRate will be equivalent to y, and yawRate will be equivalent to x

#include "Ewma.h" 
#include "utils.h"

//returns calculated pitchRate or yawRate
/**
 * @brief Calculate Pitch or Yaw rate given raw inputs and current roll orientation
 * 
 * @note Imagine your rocket has a sensor on it that detects X and Y pitch. If the Vehicle rolls 90 degrees, these sensors will become swapped. 
 *    If the vehicle rolls 180 degrees, the sensors will be reading the exact opposite of reality. This function normalizes the input using simple trig.
 * 
 * @param roll current roll orientation
 * @param parallel raw rate in one direction (X)
 * @param perpendicular raw rate in perpendicular direction (Y)
 * @return double actual rate in cardinal X direction
 */
double getRate(double roll, double parallel, double perpendicular){//if getting rateX, parallel should be x
  return parallel * cos(roll) + perpendicular * sin(roll);
}

/**
 * @brief Filter raw data through an EWMA to compute "Zero Rate Offset" for each axis
 * 
 * @param gyro One sample of the gyroscope
 * @param hasLaunched has the rocket launched yet
 * @return Directional current calculated zero rate offsets
 */
Directional calibrateGyro(Directional gyro, bool hasLaunched){//returns gyro offsets
  static Ewma xFilter(.002);
  static Ewma yFilter(.002);
  static Ewma zFilter(.002);
  static Directional lastSave;//0-5 second old data
  static Directional oldSave;//5-10 second old data
  static uint16_t counter = 0;

  if(hasLaunched){//once launch occurs we lock in the offsets (and return early to skip the math)
    //in order to offset the delay between launch and launch detection, we use the data from 5-10 seconds prior
    return oldSave;
  }

  Directional current;

  current.x = xFilter.filter(gyro.x);
  current.y = yFilter.filter(gyro.y);
  current.z = zFilter.filter(gyro.z);

  counter = (counter + 1) % 500;
  //every 500 ticks (5 seconds), we save the last state and transfer the previous state to the go queue
  if(counter == 0){
    oldSave = lastSave;
    lastSave = current;
  }
  return oldSave;
}

/**
 * @brief Given raw gyro data and zero rate offsets, determine actual rotation rates
 * 
 * @param gyro raw gyro sample
 * @param offsets vector of zero rate offsets
 * @return Directional actual rotation rates
 */
Directional getRealGyro(Directional gyro, Directional offsets){
  Directional realGyro;
  realGyro.x = gyro.x - offsets.x;
  realGyro.y = gyro.y - offsets.y;
  realGyro.z = gyro.z - offsets.z;
  return realGyro;
}

/**
 * @brief Calculate current attitude (orientation)
 * 
 * @note In order to minimize error, we should assume that attitude is 0,0,0 at launch. However, 
 *  we don't actually know exactly when launch occurs. We can assume with almost 100% 
 *  certianty that launch is detected less than 5 seconds after it has actually occured, so 
 *  as a substitute, we can start recording attitude changes 5 seconds before detected 
 *  launch. In order to achieve this, we can record changes in 5 second increments (chunks) 
 *  prior to launch. Once launch is detected we will 'lock in' those chunks and continue 
 *  recording changes forever.
 * 
 * @param gyro Calibrated gyro sample
 * @param hasLaunched has the rocket launched
 * @return Directional current attitude vector (Euler angles)
 */
Directional getAttitude(Directional gyro, bool hasLaunched){//only calibrated gyro data should go in here
  const int hz = 100; //number of readings per second
  static Directional oldChanges; //5-10 seconds ago
  static Directional lastChanges; //0-5 seconds ago
  static uint16_t counter = 0;
  Directional currentAttitude;

  counter = (counter + 1) % 500;

  lastChanges.z += gyro.z/hz;
  currentAttitude.z = oldChanges.z + lastChanges.z; //z must be set first so we can calculate x and y
  lastChanges.x += getRate(currentAttitude.z/* -gyro.z/2 */, gyro.x, gyro.y)/hz; //TODO: test whether or not -gyro.z/2 makes a difference
  lastChanges.y += getRate(currentAttitude.z/* -gyro.z/2 */, gyro.y, gyro.x)/hz;
  currentAttitude.x = oldChanges.x + lastChanges.x;
  currentAttitude.y = oldChanges.y + lastChanges.y;

  if(counter == 0 && !hasLaunched){//every 5 seconds we dump all data that's more than 5 seconds old (prior to launch)
    oldChanges = lastChanges;
    lastChanges.x = 0;
    lastChanges.y = 0;
    lastChanges.z = 0;
  }

  return currentAttitude;
}
