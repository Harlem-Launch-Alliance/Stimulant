#include <Wire.h> //library allows communication with I2C / TWI devices
#include <math.h> //library includes mathematical functions

const int MPU=0x68;                                 //I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;                //16-bit integers
int AcXcal,AcYcal,AcZcal,GyXcal,GyYcal,GyZcal,tcal; //calibration variables
double tf,tc,pitch,roll,Ax,Ay,Az,Gx,Gy,Gz;

void setupIMU(){
  //Acceleration data correction
  AcXcal = -950;
  AcYcal = -300;
  AcZcal = 0;
  //Temperature correction
  tcal = -1600;
  //Gyro correction
  GyXcal = 480; //480
  GyYcal = 170; //170
  GyZcal = 210; //210
  
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

void getIMU()
{
  Wire.beginTransmission(MPU);    //begin transmission to I2C slave device
  Wire.write(0x3B);               // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);    //restarts transmission to I2C slave device
  Wire.requestFrom(MPU,14,true);  //request 14 registers in total  

  //read accelerometer data
  AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) 0x3C (ACCEL_XOUT_L)  
  AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) 0x3E (ACCEL_YOUT_L) 
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) 0x40 (ACCEL_ZOUT_L)
  Ax = (AcX + AcXcal)/4096.0;     // For AFS_SEL=2 value is 4096
  Ay = (AcY + AcYcal)/4096.0;
  Az = (AcZ + AcZcal)/4096.0;

  //read temperature data 
  Tmp = Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) 0x42 (TEMP_OUT_L)
  double tx = Tmp + tcal;
  tc = tx/340 + 36.53;        // temperature in degrees C from datasheet
  tf = (tc * 9/5) + 32;              // temperature in fahrenheit

  //read gyroscope data
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) 0x48 (GYRO_ZOUT_L) 
  Gx = (GyX + GyXcal)/131.0;      // For FS_SEL=0 value is 131
  Gy = (GyY + GyYcal)/131.0;
  Gz = (GyZ + GyZcal)/131.0;

  //get pitch/roll
  getAngle(AcX,AcY,AcZ);
}

//function to convert accelerometer values into pitch and roll
void getAngle(int Ax,int Ay,int Az) 
{
  double x = Ax;
  double y = Ay;
  double z = Az;
  pitch = atan(x/sqrt((y*y) + (z*z)));
  roll = atan(y/sqrt((x*x) + (z*z)));
  //converting radians into degrees
  pitch = pitch * (180.0/3.14);
  roll = roll * (180.0/3.14) ;
}

void printData()
{
  Serial.print("Angle: ");
  Serial.print("Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.println(roll);

  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(Ax);
  Serial.print(" Y = "); Serial.print(Ay);
  Serial.print(" Z = "); Serial.println(Az); 

  Serial.print("Temperature in celsius = "); Serial.print(tc);  
  Serial.print(" fahrenheit = "); Serial.println(tf);  
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(Gx);
  Serial.print(" Y = "); Serial.print(Gy);
  Serial.print(" Z = "); Serial.println(Gz);
}

void setupIMU()
{
  Wire.begin();         //initiate wire library and I2C
  setupIMU();
  Serial.begin(9600);   //serial communication at 9600 bauds
}

//void loop()
//{
//  getIMU();
//  printData();
//}
