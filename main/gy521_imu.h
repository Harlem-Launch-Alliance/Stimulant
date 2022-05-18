/*****************************************************************************
 * This file is for all imu queries
 * Calculations such as attitude determination should go in a separate file
 ****************************************************************************/


#include <Wire.h> //library allows communication with I2C / TWI devices
#include <math.h> //library includes mathematical functions

const int MPU=0b1101000;                                 //I2C address of the MPU-6050
const double tcal = -1600;                          //Temperature correction

imuReading getIMU()
{
  imuReading imuSample;
  imuSample.time = micros(); //current timestamp
  int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, Tmp;

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,1);

  //read accelerometer data
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  //read temp
  Tmp=Wire.read()<<8|Wire.read();
  //read gyroscope data
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();

  
  imuSample.accel.x = AcX/16384.0;
  imuSample.accel.y = AcY/16384.0;
  imuSample.accel.z = AcZ/16384.0;
  imuSample.gyro.x = toRad(GyX/131.0);
  imuSample.gyro.y = toRad(GyY/131.0);
  imuSample.gyro.z = toRad(GyZ/131.0);
  //imuSample.temp = Tmp/340 + 36.53;
  
  return imuSample; //return gyro and accel data
}

void setupIMU(){ 
  Serial1.print("Setting up IMU... ");
  delay(3000);
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0b00010000); //Setting the gyro to full scale +/- 1000deg./s 
  Wire.endTransmission(); 
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00010000); //Setting the accel to +/- 8g
  Wire.endTransmission();


  imuReading sample = getIMU();
  if(sample.accel.x != 0 || sample.accel.y != 0 || sample.accel.z != 0)
    Serial1.println("successful");
  else
    Serial1.println("failed, check wiring or pin settings");
}
