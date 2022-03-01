/*****************************************************************************
 * This file is for all math used to determine attitude
 * 
 ****************************************************************************/

//angles are in radians
//rates are magnitudes of rad/s
//GyX, GyY, GyZ are readings from gyro
//pitch, yaw, roll, pitchRate, yawRate are calculated based on readings
//assuming roll == 0, pitchRate will be equivalent to GyY, and yawRate will be equivalent to GyX

#include <Ewma.h> //https://github.com/jonnieZG/EWMA

//returns calculated pitchRate or yawRate
double getRate(double roll, double parallel, double perpendicular){//parallel would be GyY to get pitchRate, with perpendicular being GyX
  return parallel * cos(roll) + perpendicular * sin(roll);
}

Directional calibrateGyro(Directional gyro, bool hasLaunched){//returns gyro offsets
  static Ewma xFilter(.02);
  static Ewma yFilter(.02);
  static Ewma zFilter(.02);
  static Directional lastSave;
  static Directional oldSave;
  static int counter = 0;

  if(hasLaunched){//once launch occurs we lock in the offsets
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
