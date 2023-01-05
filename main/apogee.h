/*****************************************************************************
 * apogee detection and helper functions
 * 
 ****************************************************************************/

#include "Ewma.h"
#include "utils.h"

#define G_FORCE_TO_LAUNCH 3 //if acceleration exceeds this number the rocket will assume it has been launched
#define MAX_APOGEE_ACCEL 2 //we can rule out apogee if acceleration is about this amount (Gs)
//#define CHECK_FOR_APOGEE_HZ 4 //frequency that we check for apogee within the detect apogee function

//this function should only be called once per loop iteration
bool detectLaunch(Directional accel){//accel in Gs 
  static bool hasLaunched = false;
  static Ewma accelFilter(.05);
  
  if(hasLaunched) //no need to do any math once launch has been detected the first time
    return true;

  //group all accels into one
  //accel may need calibration. If that is the case, it should happen externally and only calibrated data should enter this function
  double totalAccel = sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)); //this should be 1G when stationary
  
  double filteredAccel = accelFilter.filter(totalAccel);
  if(filteredAccel > G_FORCE_TO_LAUNCH)
    hasLaunched = true;
  return hasLaunched;
}

bool isAccelerating(Directional accel){//determine if we are accelerating at more than 2 Gs
  return sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)) > MAX_APOGEE_ACCEL;
}

bool detectApogee(Directional accel, double altitude, bool hasLaunched){ //accel in Gs
  static bool apogeeReached = false; //this is initialized as false, but once it flips to true it should remain true until the arduino is reset
  static int counter = 0;
  counter = (counter + 1) % 5; //this will count from 0 to 4, overflowing back to 0
  static double lastAlt = altitude;
  
  if(apogeeReached)//if apogee was already detected earlier we don't need to do any more math
    return true;

  if(counter == 0){
    //everytime counter overflows we check for apogee (4hz at the moment, this should be a constant or tied to a constant)
    if(hasLaunched && altitude < lastAlt  && !isAccelerating(accel)){
      apogeeReached = true;
    }
    lastAlt = altitude;
  }
  return apogeeReached;
}
