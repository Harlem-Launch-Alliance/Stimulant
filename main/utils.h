/*****************************************************************************
 * This file is for declaring new datatypes
 * 
 ****************************************************************************/


struct Directional  //any data that has an x,y and z attribute
{
  double x,y,z; 
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
