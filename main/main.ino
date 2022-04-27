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



// Constants:
#define SEALEVELPRESSURE_HPA (1013.25)
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 33;
unsigned long lastTime = 0;

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

  setupGPS();
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

flightPhase runOnPad(int tick);
flightPhase runAscending(int tick);
flightPhase runDescending(int tick);
flightPhase runPostFlight(int tick);

void loop()
{
  static int tick = 0;
  static flightPhase status = ONPAD;
  unsigned int tickTime = getTickTime(status);
  while(micros() - lastTime < tickTime){
    //waste time
    int x = 1 + 1;
  }
  lastTime = micros();

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
  //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old

  bool hasLaunched = detectLaunch(imuSample.accel); //might need to calibrate accel data before feeding to this function
  if(tick % 5 == 0){
    lastBmp = getBMP();
    detectApogee(imuSample.accel, lastBmp.altitude, hasLaunched); //need to run this prelaunch to get calibrations
    //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old
  }

  Directional gyroOffsets = calibrateGyro(imuSample.gyro, hasLaunched);
  Directional calibratedGyro = getRealGyro(imuSample.gyro, gyroOffsets);
  Directional attitude = getAttitude(calibratedGyro, hasLaunched);

  if(tick % 50 == 1){//twice per second with a slight offset to avoid overlapping with bmp
    //TODO adjust data transmission to altitude, GPS, and attitude only
    transmitData(0, 0, 0, imuSample.accel, imuSample.gyro, apogeeReached);
  }
  if(tick % 100 == 2){//1 time per second with a slight offset to avoid collisions
    lastGps = getGPS();
    //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old
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
  //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old

  if(tick % 5 == 0){
    lastBmp = getBMP();
    apogeeReached = detectApogee(imuSample.accel, lastBmp.altitude, true);
    //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old
  }
  if(tick % 100 == 2){//1 time per second with a slight offset to avoid collisions
    lastGps = getGPS();
    //TODO push reading onto queue. if we haven't launched pop values off of queue until first value is less than 5 seconds old
  }

  Directional gyroOffsets = calibrateGyro(imuSample.gyro, true);
  Directional calibratedGyro = getRealGyro(imuSample.gyro, gyroOffsets);
  Directional attitude = getAttitude(calibratedGyro, true);

  if(tick % 5 == 1){//20 times per second with a slight offset to avoid overlapping with bmp
    //TODO adjust data transmission to altitude, GPS, and attitude only
    transmitData(0, 0, 0, imuSample.accel, imuSample.gyro, apogeeReached);
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
  //TODO push directly onto SD

  //sample sensors
  bmpReading bmpSample = getBMP();

  if(tick % 20 == 1){//still one time per second
    lastGps = getGPS();
    //TODO push directly onto SD
  }
  //TODO adjust data transmission to altitude, GPS, and attitude only
  //transmitData(0, 0, 0, imuSample.accel, imuSample.gyro, true);

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
  if(tick % 5 == 0){//broadcast every 5 seconds
    gpsReading gpsSample = getGPS();
    //TODO adjust data transmission to altitude, GPS, and attitude only
    //transmitData(0, 0, 0, imuSample.accel, imuSample.gyro, true);
  }
  return POST_FLIGHT;
}
