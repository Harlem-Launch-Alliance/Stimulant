/*************************************************************************************
                                 Harlem Launch Alliance
                            Recovery Systems Group 2021 - 2022
                              Catalyst 2.1 Flight Computer
 *************************************************************************************/
#include <Wire.h>             // to communicate with I2C devices
#include <SD.h>               // for data storage on the onboard microSD card
#include "Adafruit_BMP3XX.h"  // BMP388 library
#include <math.h>             // for IMU angle conversion
#include <list>               // for apogee detection
#include <cmath>              // for apogee detection

// Constants:
#define SEALEVELPRESSURE_HPA (1013.25)
const int MPU=0x68;                                 //I2C address of the MPU-6050
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 33;
const int TIMEFRAME = 3; // should detect apogee about TIMEFRAME/3 seconds after apogee
const int SAMPLERATE = 20; //data points per second

// Objects:
Adafruit_BMP3XX bmp;
File dataFile;

// Variables:
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;                      // 16-bit integers to store raw IMU data
int AcXcal,AcYcal,AcZcal,GyXcal,GyYcal,GyZcal,tcal;       // calibration variables
double tc,pitch,roll,Ax,Ay,Az,Gx,Gy,Gz;                   // final values
double Altitude, temperature, pressure, altOffset = 0;    // final values
float Time;

void setupIMU(){
  Serial1.print("Setting up GY521: ");
  //Acceleration data correction
  AcXcal = -950;
  AcYcal = -300;
  AcZcal = 0;
  //Temperature correction
  tcal = -1600;
  //Gyro correction
  GyXcal = 480;
  GyYcal = 170;
  GyZcal = 210;
  
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
  Serial1.println("successful");
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
  tc = tx/340 + 36.53;              // temperature in degrees C from datasheet

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
  pitch = pitch * (180.0/3.14);           // converting radians into degrees
  roll = roll * (180.0/3.14) ;
}

void setupBMP()
{
  Serial1.print("Setting up BMP388: ");
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial1.println("could not find a valid BMP388 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  for(int i = 0; i < 100; i++)                             // runs BMP 100 times to flush out data
    getBMP();
  Serial1.println("successful");
}

void getBMP()
{
  if (! bmp.performReading()) {
    Serial1.println("Failed to perform BMP388 reading.");
    return;
  }
  temperature = bmp.temperature;          // Temperature in Celcius
  pressure = bmp.pressure / 100.0;        // Pressure in hPa
  Altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - altOffset; // Approximate altitude in meters
}


bool hasLaunched(double accelX,  double accelY, double accelZ){
  const int dataPoints = 5;
  static bool hasLaunched = false;
  if(hasLaunched)
    return true;
  static std::list<double> totalAccel;
  std::list<double>::iterator it;
  double currentTotal = sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));
  totalAccel.push_front(currentTotal);
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
  static bool apogeeReached = false;
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

void transmitData()
{
  Serial1.print(Time); Serial1.print("\t");
  Serial1.print(tc); Serial1.print("\t");
  Serial1.print(temperature); Serial1.print("\t");
  Serial1.print(pitch); Serial1.print("\t");
  Serial1.print(roll); Serial1.print("\t");
  Serial1.print(Ax); Serial1.print("\t");
  Serial1.print(Ay); Serial1.print("\t");
  Serial1.print(Az); Serial1.print("\t");
  Serial1.print(Gx); Serial1.print("\t");
  Serial1.print(Gy); Serial1.print("\t");
  Serial1.print(Gz); Serial1.print("\t");
  Serial1.print(pressure); Serial1.print("\t"); Serial1.print("\t");    
  Serial1.print(Altitude); Serial1.print("\t");
  Serial1.println(apogeeReached);
}
void recordData()
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(Time); dataFile.print("\t");
    dataFile.print(tc); dataFile.print("\t");
    dataFile.print(temperature); dataFile.print("\t");
    dataFile.print(pitch); dataFile.print("\t");
    dataFile.print(roll); dataFile.print("\t");
    dataFile.print(Ax); dataFile.print("\t");
    dataFile.print(Ay); dataFile.print("\t");
    dataFile.print(Az); dataFile.print("\t");
    dataFile.print(Gx); dataFile.print("\t");
    dataFile.print(Gy); dataFile.print("\t");
    dataFile.print(Gz); dataFile.print("\t");
    dataFile.print(pressure); dataFile.print("\t"); dataFile.print("\t");
    dataFile.print(Altitude); dataFile.print("\t");
    dataFile.println(apogeeReached);
    dataFile.close();                       // closing file is very important
  }
  else
    Serial1.println("error opening datalog.txt");
}

void setup()
{
  pinMode(buzzer, OUTPUT);    // Set buzzer pin as an output
  for(int i = 0; i < 5; i++)  // Play 5 beeps
  {
    tone(buzzer, 1000);       // Send 1KHz sound signal...
    delay(1000);              // ...for 1 sec
    noTone(buzzer);           // Stop sound...
    delay(1000);              // ...for 1sec
  }
  Wire.begin();               // initiate wire library and I2C
  Serial1.begin(115200);      // xBee 115200, 9600
  setupIMU();
  setupBMP();
  
  // set up sd card
  if (!SD.begin(chipSelect))
    Serial1.println("MicroSD card: failed or not present");
  else
    Serial1.println("MicroSD card: successful");
  
  // calculating the initial altitude on the ground to use as offset
  double total = 0;
  for(int i = 0; i < 100; i++)
  {
    getBMP();
    total += Altitude;
  }
  altOffset = total/100.0;
  
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Catalyst 2 Flight Computer Test December 14th, 2021");
    //dataFile.println("Single deployment level 1 rocket flight data");
    dataFile.println("T(s) IMU_T(C) BMP_T(C) Pitch(deg) Roll(deg) Ax(g) Ay Az Gx Gy Gz P(hPa) A(m) Apogee");
    dataFile.close();
  }
  else {
    Serial1.println("error opening datalog.txt");
  }
}

void loop()
{
  Time = float(millis()) / 1000.0;
  getIMU();
  getBMP();
  detectApogee(Ax, Ay, Az, Altitude);
  recordData();
  transmitData();
}
