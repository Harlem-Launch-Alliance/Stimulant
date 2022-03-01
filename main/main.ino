/*************************************************************************************
                                 Harlem Launch Alliance
                            Recovery Systems Group 2021 - 2022
                              Catalyst 2.1 Flight Computer
 *************************************************************************************/
#include <Wire.h>             // to communicate with I2C devices
#include <SD.h>               // for data storage on the onboard microSD card
#include "Adafruit_BMP3XX.h"  // BMP388 library
#include "utils.h"
#include "apogee.h"
#include "bmp_altimeter.h"
#include "data.h"
#include "gy521_imu.h"



// Constants:
#define SEALEVELPRESSURE_HPA (1013.25)
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 33;

// Variables:
bool apogeeReached;

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
  setupBMP();
  
  // set up sd card
  if (!SD.begin(chipSelect))
    Serial1.println("MicroSD card: failed or not present");
  else
    Serial1.println("MicroSD card: successful");

  //setupGPS();
  setupIMU();
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Catalyst 2 Flight Computer Test December 14th, 2021"); //TODO we can use GPS to get date
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
  //time = float(millis()) / 1000.0;
  bmpReading bmpSample = getBMP();
  imuReading imuSample = getIMU();
  bool hasLaunched = detectLaunch(imuSample.accel); //might need to calibrate accel data before feeding to this function
  apogeeReached = detectApogee(imuSample.accel, bmpSample.altitude, hasLaunched);
  transmitData(bmpSample.temp, bmpSample.pressure, bmpSample.altitude, imuSample.accel, imuSample.gyro, apogeeReached);
  recordData();
}
