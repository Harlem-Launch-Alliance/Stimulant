//apogee detection and helper functions
#include <list>
#include <cmath>

const int TIMEFRAME = 3; // should detect apogee about TIMEFRAME/3 seconds after apogee
const int SAMPLERATE = 20; //data points per second

bool hasLaunched(double accelX,  double accelY, double accelZ){
  const int dataPoints = 5;
  static bool hasLaunched = false;
  if(hasLaunched)
    return true;
  static std::list<double> totalAccel;
  std::list<double>::iterator it;
  double currentTotal = sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));
  totalAccel.push_front(currentTotal); //maintain a rolling list of past accel data
  //cleanup old data
  if(totalAccel.size() > dataPoints){
    totalAccel.pop_back();
  }
  hasLaunched = true;
  for(it = totalAccel.begin(); it != totalAccel.end(); it++){
    if(*it < 3 || totalAccel.size() < dataPoints){
      hasLaunched = false;
    }
  }
  return hasLaunched;
}

bool detectApogee(double accelX, double accelY, double accelZ, double altitude){ //accel in Gs
  if(!hasLaunched(accelX, accelY, accelZ))
    return false;
  const int dataPoints = TIMEFRAME * SAMPLERATE;
  static bool apogeeReached = false; //this is initialized as false, but once it flips to true it should remain true until the arduino is reset
  static std::list<double> altitudes;
  std::list<double>::iterator it;

  if(apogeeReached)
    return true;
  
  altitudes.push_front(altitude);
  if(altitudes.size() > dataPoints){
    altitudes.pop_back();
  }
  if(altitudes.size() < dataPoints){
    return false;
  }
  double first3rd, last3rd = 0;
  for(it = altitudes.begin(); std::distance(altitudes.begin(),it) < dataPoints/3; it++){
    first3rd += *it;
  }
  for(it = altitudes.end(); std::distance(it, altitudes.end()) <= dataPoints/3; it--){
    last3rd += *it;
  }
  first3rd = first3rd/ (dataPoints/3);
  last3rd = last3rd/ (dataPoints/3);
  if(last3rd > first3rd && sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2)) < 2){
    apogeeReached = true;
  }
  return apogeeReached;
}