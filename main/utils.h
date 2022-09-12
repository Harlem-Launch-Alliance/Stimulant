/*****************************************************************************
 * This file is for declaring utilities to be used in multiple other places
 * 
 ****************************************************************************/
#include <math.h>
#include "settings.h"

class Directional  //any data that has an x,y and z attribute
{
public:
  double x,y,z; 
  Directional(double a, double b, double c){
    x = a;
    y = b;
    z = c;
  }
  Directional(){
    x = 0;
    y = 0;
    z = 0;
  }
};

struct bmpReading //all data from an altimeter reading
{
  double altitude;
  unsigned long time;
  unsigned short state; //used for tracking flightPhase (0-3 representing ONPAD - POST_FLIGHT)
};

struct imuReading //all data from an IMU reading (accelerometer and gyroscope)
{
    Directional accel;
    Directional gyro;
    Directional attitude;
    unsigned long time;
};

struct gpsReading //only the data we need for now
{
    double latitude, longitude;
    unsigned long time;
};

double toDeg(double angle){
  return ((angle) *(180/PI));
}

double toRad(double angle){
  return ((angle) * (PI/180));
}

enum flightPhase {ONPAD, ASCENDING, DESCENDING, POST_FLIGHT};

int getTickTime(flightPhase phase){//map flight phases to tick times
  switch(phase){
  case ONPAD:
    return TICK_TIME_ONPAD;
  case ASCENDING:
    return TICK_TIME_ASCENDING;
  case DESCENDING:
    return TICK_TIME_DESCENDING;
  case POST_FLIGHT:
    return TICK_TIME_POST_FLIGHT;
  }
  return TICK_TIME_POST_FLIGHT;
}
