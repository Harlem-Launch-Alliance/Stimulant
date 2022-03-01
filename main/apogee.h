/*****************************************************************************
 * apogee detection and helper functions
 * 
 ****************************************************************************/

#include <LinkedList.h> //https://github.com/ivanseidel/LinkedList
#include <Ewma.h> //https://github.com/jonnieZG/EWMA

bool hasLaunched(Directional accel){
  const int dataPoints = 5;
  static bool hasLaunched = false;
  if(hasLaunched)
    return true;
  static LinkedList<double> totalAccel;
  double currentTotal = sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2));
  totalAccel.unshift(currentTotal); //maintain a rolling list of past accel data
  //cleanup old data
  if(totalAccel.size() > dataPoints){
    totalAccel.pop();
  }
  hasLaunched = true;
  for(int i = 0; i < totalAccel.size(); i++){
    if(totalAccel.get(i) < 3 || totalAccel.size() < dataPoints){
      hasLaunched = false;
    }
  }
  return hasLaunched;
}

bool isAccelerating(Directional accel){
  return sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)) < 2;
}

bool detectApogee(Directional accel, double altitude){ //accel in Gs
  if(!hasLaunched(accel))
    return false;
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
    if(currentAlt < lastAlt && !isAccelerating(accel)){
      apogeeReached = true;
    }
    lastAlt = currentAlt;
  }
  return apogeeReached;
}
