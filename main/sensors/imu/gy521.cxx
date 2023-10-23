/*****************************************************************************
 * This file is for all imu queries from the GY521
 * Calculations such as attitude determination should go in a separate file
 ****************************************************************************/

#ifdef GY521

#include "imu.h"
#include "../../utils/utils.h"

#include <math.h> //library includes mathematical functions
#include <Wire.h> //library allows communication with I2C / TWI devices

const uint8_t MPU=0x68;                  //I2C address of the MPU-6050
const double tcal = -1600;           //Temperature correction

imuReading imu::sample()
{
  imuReading imuSample;
  imuSample.time = micros(); //current timestamp
  int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, Tmp;

  (void) Tmp; //we can remove this if we decide to record temperature from the IMU

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,(uint8_t) 14,true);

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
  
  imuSample.accel.x = AcX/4096.0;
  imuSample.accel.y = AcY/4096.0;
  imuSample.accel.z = AcZ/4096.0;
  imuSample.gyro.x = toRad(GyX/131.0);
  imuSample.gyro.y = toRad(GyZ/131.0);
  imuSample.gyro.z = toRad(GyY/131.0); //IMU's Y corresponds to rocket's Z axis due to orientation on PCB
  //imuSample.temp = Tmp/340 + 36.53;     // Not using IMU's temp sensor since BMP388 has one

  return imuSample; //return gyro and accel data
}

/**
 * @brief Establish I2C connection to the IMU.
 * 
 * Verify that the connection is operating properly by taking a sample.
 */
void imu::setup(){ 
  XBeeSerial.print("Setting up GY521 IMU... ");
  
  Wire.beginTransmission(MPU);  //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B);             //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000);       //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  
  Wire.beginTransmission(MPU); //I2C address of the MPU
  Wire.write(0x1B);            //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0b00010000);      //Setting the gyro scale to +/- 250 deg/s 
  Wire.endTransmission(); 
  
  Wire.beginTransmission(MPU);  //I2C address of the MPU
  Wire.write(0x1C);             //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00010000);       //Setting the accel to +/- 8g
  Wire.endTransmission();

  delay(3000);                  // This delay allows time for the IMU setup to be completed before reading values.
  imuReading sample = imu::sample(); // Reads 1 sample of data from IMU.

  if(sample.accel.x != 0 || sample.accel.y != 0 || sample.accel.z != 0)
    XBeeSerial.println("IMU setup successful.");
  else{
    XBeeSerial.println("Setup failed, did not read acceleration values from IMU.");
  } 

XBeeSerial.print("X Acceleration: ");
XBeeSerial.println(sample.accel.x);
delay(1000);
XBeeSerial.print("Y Acceleration: ");
XBeeSerial.println(sample.accel.y);
delay(1000);
XBeeSerial.print("Z Acceleration: ");
XBeeSerial.println(sample.accel.z);
delay(1000);
XBeeSerial.print("X Gyro: ");
XBeeSerial.println(sample.gyro.x);
delay(1000);
XBeeSerial.print("Y Gyro: ");
XBeeSerial.println(sample.gyro.y);
delay(1000);
XBeeSerial.print("Z Gyro: ");
XBeeSerial.println(sample.gyro.z);
delay(1000);

}

#endif // GY521
