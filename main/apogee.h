/*****************************************************************************
 * apogee detection and helper functions
 * 
 ****************************************************************************/

#include <LinkedList.h> //https://github.com/ivanseidel/LinkedList


const int TIMEFRAME = 3; // should detect apogee about TIMEFRAME/3 seconds after apogee
const int SAMPLERATE = 20; //data points per second

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

bool detectApogee(Directional accel, double altitude){ //accel in Gs
  if(!hasLaunched(accel))
    return false;
  const int dataPoints = TIMEFRAME * SAMPLERATE;
  static bool apogeeReached = false; //this is initialized as false, but once it flips to true it should remain true until the arduino is reset
  static LinkedList<double> altitudes;
  
  if(apogeeReached)
    return true;
  
  altitudes.unshift(altitude);
  if(altitudes.size() > dataPoints){
    altitudes.pop();
  }
  if(altitudes.size() < dataPoints){
    return false;
  }
  double first3rd, last3rd = 0;
  for(int i = 0; i < dataPoints/3; i++){
    first3rd += altitudes.get(i);
  }
  for(int i = dataPoints - 1; dataPoints - i < dataPoints/3; i++){
    last3rd += altitudes.get(i);
  }
  first3rd = first3rd/ (dataPoints/3); //first3rd and last3rd refer indices of respective dataPoints
  last3rd = last3rd/ (dataPoints/3);
  if(last3rd > first3rd && sqrt(pow(accel.x, 2) + pow(accel.y, 2) + pow(accel.z, 2)) < 2){
    apogeeReached = true;
  }
  return apogeeReached;
}
