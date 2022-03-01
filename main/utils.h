/*****************************************************************************
 * This file is for declaring new datatypes
 * 
 ****************************************************************************/


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
  double temp, pressure, altitude;
  unsigned long time;
};

struct imuReading //all data from an IMU reading (accelerometer and gyroscope)
{
    Directional accel;
    Directional gyro;
    unsigned long time;
    double temp;
};
