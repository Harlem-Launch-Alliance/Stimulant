/*****************************************************************************
 * This file is for all imu queries
 * Calculations such as attitude determination should go in a separate file
 ****************************************************************************/


#include <Wire.h> //library allows communication with I2C / TWI devices
#include <math.h> //library includes mathematical functions

const int MPU=0x68;                                 //I2C address of the MPU-6050
const double tcal = -1600;                          //Temperature correction

void setupIMU(){ 
  Wire.beginTransmission(MPU);          //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B);                     //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000);               //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  
  Wire.beginTransmission(0b1101000);    //I2C address of the MPU
  Wire.write(0x1B);                     //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000);               //Setting the gyro to FS_SEL=0 (+/- 250 deg/s) 
  Wire.endTransmission(); 
  
  Wire.beginTransmission(0b1101000);    //I2C address of the MPU
  Wire.write(0x1C);                     //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00010000);               //Setting the accel to AFS_SEL=2 (to +/- 8g)
  Wire.endTransmission();
}

imuReading getIMU()
{
  Wire.beginTransmission(MPU);    //begin transmission to I2C slave device
  Wire.write(0x3B);               // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);    //restarts transmission to I2C slave device
  Wire.requestFrom(MPU,14,1);  //request 14 registers in total  

  imuReading imuSample;

  //read accelerometer data
  int16_t AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) 0x3C (ACCEL_XOUT_L)   //16bit integer
  int16_t AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) 0x3E (ACCEL_YOUT_L) 
  int16_t AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) 0x40 (ACCEL_ZOUT_L)
  imuSample.accel.x = (AcX /*+ AcXcal*/)/4096.0;     // For AFS_SEL=2 value is 4096
  imuSample.accel.y = (AcY /*+ AcYcal*/)/4096.0;
  imuSample.accel.z = (AcZ /*+ AcZcal*/)/4096.0;

  //read temperature data 
  int16_t Tmp = Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) 0x42 (TEMP_OUT_L)
  double tx = Tmp + tcal;
  double tc = tx/340 + 36.53;        // temperature in degrees C from datasheet
  double tf = (tc * 9/5) + 32;              // temperature in fahrenheit

  imuSample.temp = tf;
  
  //read gyroscope data
  int16_t GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) 0x44 (GYRO_XOUT_L)
  int16_t GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) 0x46 (GYRO_YOUT_L)
  int16_t GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) 0x48 (GYRO_ZOUT_L) 
  imuSample.gyro.x = (GyX /*+ GyXcal*/)/131.0;      // For FS_SEL=0 value is 131
  imuSample.gyro.y = (GyY /*+ GyYcal*/)/131.0;
  imuSample.gyro.z = (GyZ /*+ GyZcal*/)/131.0;
  imuSample.time = micros();

  return imuSample;
}
