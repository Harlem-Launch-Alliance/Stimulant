/*****************************************************************************
 * apogee detection and helper functions
 * 
 ****************************************************************************/

#include <Ewma.h> //https://github.com/jonnieZG/EWMA

//this function should only be called once per loop iteration
bool detectLaunch(Directional accel){//accel in Gs 
  const double gForceToLaunch = 3; //if acceleration exceeds this number the rocket will assume it has been launched
  static bool hasLaunched = false;
  static Ewma accelFilter(.05);
  
  if(hasLaunched) //no need to do any math once launch has been detected the first time
    return true;

  //group all accels into one
  //accel may need calibration. If that is the case, it should happen externally and only calibrated data should enter this function
  double totalAccel = sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)); //this should be 1G when stationary
  
  double filteredAccel = accelFilter.filter(totalAccel);
  if(filteredAccel > gForceToLaunch)
    hasLaunched = true;
  return hasLaunched;
}

bool isAccelerating(Directional accel){
  return sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)) < 2;
}

bool detectApogee(Directional accel, double altitude, bool hasLaunched){ //accel in Gs
  static bool apogeeReached = false; //this is initialized as false, but once it flips to true it should remain true until the arduino is reset
  static Ewma altFilter(.05); //this will contain a filtered altitude (less prone to noise)
  static int counter = 0;
  static double lastAlt = altitude;
  counter = (counter + 1) % 10; //this will count from 0 to 9, overflowing back to 0
  double currentAlt = altFilter.filter(altitude);
  
  if(apogeeReached)//if apogee was already detected earlier we don't need to do any more math
    return true;
  
  if(counter == 0){
    //every 10 readings we'll check for apogee
    if(hasLaunched && currentAlt < lastAlt && !isAccelerating(accel))
      apogeeReached = true;
    lastAlt = currentAlt;
  }
  return apogeeReached;
}
