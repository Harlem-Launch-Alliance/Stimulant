/*************************************************************************************
                                 Harlem Launch Alliance
                            Recovery Systems Group 2021 - 2022
                              Catalyst 2.1 Flight Computer
 *************************************************************************************/
#include <Wire.h>             // to communicate with I2C devices
#include <SD.h>               // for data storage on the onboard microSD card
#include "utils.h"
#include "Adafruit_BMP3XX.h"  // BMP388 library
#include "apogee.h"
#include "bmp_altimeter.h"
#include "data.h"
#include "gy521_imu.h"
#include "attitude.h"
#include "gps.h"
#include "ringQueue.h"

// Constants:
#define SEALEVELPRESSURE_HPA (1013.25)
const int buzzer = 33;
unsigned long lastTime = 0;

// Variables:
bool apogeeReached;

void setup()
{
//  pinMode(buzzer, OUTPUT);    // Set buzzer pin as an output
//  for(int i = 0; i < 5; i++)  // Play 5 beeps
//  {
//    tone(buzzer, 1000);       // Send 1KHz sound signal...
//    delay(1000);              // ...for 1 sec
//    noTone(buzzer);           // Stop sound...
//    delay(1000);              // ...for 1sec
//  }
  Wire.begin();               // initiate wire library and I2C
  Serial1.begin(115200);      // xBee baudrate: 115200, 9600
  Serial1.print("\n\n\n");
  setupBMP();
  setupIMU();

  String date = setupGPS();
  setupSD(date);
}

flightPhase runOnPad(int tick);
flightPhase runAscending(int tick);
flightPhase runDescending(int tick);
flightPhase runPostFlight(int tick);

void loop()
{
  static int tick = 0;
  static flightPhase status = ONPAD;
  unsigned int tickTime = getTickTime(status);
  if(status == POST_FLIGHT){
    while(millis() - lastTime < tickTime/1000){
      //waste time
      int x = 1 + 1;
    }
    lastTime = millis();
  } else {
    while(micros() - lastTime < tickTime){
      //waste time
      int x = 1 + 1;
    }
    lastTime = micros();
  }

  switch(status){
  case ONPAD:
    runOnPad(tick);
    break;
  case ASCENDING:
    runAscending(tick);
    break;
  case DESCENDING:
    runDescending(tick);
    break;
  case POST_FLIGHT:
  default:
    runPostFlight(tick);
  }
  tick++;
}

flightPhase runOnPad(int tick){
  //sample sensors
  static bmpReading lastBmp;
  static gpsReading lastGps;
  
  imuReading imuSample = getIMU(); //sample IMU first to maximize consistency

  bool hasLaunched = detectLaunch(imuSample.accel); //might need to calibrate accel data before feeding to this function
  if(tick % 5 == 0){
    lastBmp = getBMP();
    detectApogee(imuSample.accel, lastBmp.altitude, hasLaunched); //need to run this prelaunch to get calibrations
    recordData(lastBmp, true);
  }

  Directional gyroOffsets = calibrateGyro(imuSample.gyro, hasLaunched);
  Directional calibratedGyro = getRealGyro(imuSample.gyro, gyroOffsets);
  Directional attitude = getAttitude(calibratedGyro, hasLaunched);

  imuSample.attitude = attitude;
  recordData(imuSample, true);

  if(tick % 100 == 2 && lastGps.longitude == 0){//1 reading then use cached value
    lastGps = getGPS();
    recordData(lastGps, true);
  }
  if(tick % 50 == 1){//twice per second with a slight offset to avoid overlapping with bmp
    transmitData(lastBmp.altitude, lastGps, '0');
  }
  if(hasLaunched)
    return ASCENDING;
  return ONPAD;
}

flightPhase runAscending(int tick){ //this will run similarly to ONPAD except hasLaunched will be true
  //sample sensors
  static bmpReading lastBmp;
  static gpsReading lastGps;
  static unsigned int delay = 0;
  bool apogeeReached = false;
  imuReading imuSample = getIMU(); //sample IMU first to maximize consistency
  
  if(tick % 5 == 0){
    lastBmp = getBMP();
    apogeeReached = detectApogee(imuSample.accel, lastBmp.altitude, true);
    recordData(lastBmp, false);
  }
  Directional gyroOffsets = calibrateGyro(imuSample.gyro, true);
  Directional calibratedGyro = getRealGyro(imuSample.gyro, gyroOffsets);
  Directional attitude = getAttitude(calibratedGyro, true);

  imuSample.attitude = attitude;
  recordData(imuSample, false);

  if(tick % 5 == 1){//20 times per second with a slight offset to avoid overlapping with bmp and gps
    transmitData(lastBmp.altitude, lastGps, '1');
  }
  if(apogeeReached && !delay){
    delay = millis() + 3000;//added 3 second delay incase chute deploy is late
  }
  if(apogeeReached && delay > millis())
    return DESCENDING;
  return ASCENDING;
}

flightPhase runDescending(int tick){//this runs at 20hz
  static gpsReading lastGps;
  static int lastAlt = 0;

  //sample sensors
  bmpReading bmpSample = getBMP();
  recordData(bmpSample, false);

  if(tick % 20 == 1){//still one time per second
    lastGps = getGPS();
    recordData(lastGps, false);
  }
  if(tick % 100 == 2){//backup to SD every 5 seconds
    backupToSD();
  }
  transmitData(bmpSample.altitude, lastGps, '2');

  if(tick % 100 == 0){//every 5 seconds check if we are still descending
    if(bmpSample.altitude - lastAlt < 1){//if altitude hasn't changed more than 1 meter in 5 seconds, we're on the ground
      return POST_FLIGHT;
    }
    lastAlt = bmpSample.altitude;
  }
  return DESCENDING;
}

flightPhase runPostFlight(int tick){
  //once we're on the ground we can stop recording and start just broadcasting GPS somewhat infrequently
  static bmpReading bmpSample = getBMP();
  if(tick % 5 == 0){//broadcast every 5 seconds
    gpsReading gpsSample = getGPS();
    transmitData(bmpSample.altitude, gpsSample, '3');
  }
  return POST_FLIGHT;
}
