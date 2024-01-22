/*************************************************************************************
                                 Harlem Launch Alliance
                              Avionics Group 2021 - 2023
                                Stimulant Flight Software
 *************************************************************************************/

#include "apogee.h"
#include "attitude.h"
#include "data.h"
#include "sensors/altimeter/bmp3xx.h"
#include "sensors/gps/adafruit_gps.h"
#include "sensors/imu/imu.h"
#include "utils/utils.h"

imu imu;
Attitude attitude;

void setup()
{
  XBeeSerial.begin(115200);      // xBee baudrate: 115200, 9600
  XBeeSerial.print("\n\n\n");

  pinMode(BUZZER_PIN, OUTPUT);    // Set buzzer pin as an output
  pinMode(PYRO0_PIN, OUTPUT);     // Set pyro pins as an outputs
  pinMode(PYRO1_PIN, OUTPUT);
  digitalWrite(PYRO0_PIN, LOW);   // Set pyro pins to low
  digitalWrite(PYRO1_PIN, LOW);

  for(int i = 0; i < 5; i++)  // Play 5 beeps
  {
    tone(BUZZER_PIN, 1000);       // Send 1KHz sound signal...
    XBeeSerial.println("beep!");
    delay(1000);              // ...for 1 sec
    noTone(BUZZER_PIN);           // Stop sound...
    delay(1000);              // ...for 1sec
  }
  setupBMP();
  delay(2000);

  imu.setup();
  delay(2000);

  String date = setupGPS();
  delay(2000);

  setupSD(date);
  delay(5000);
}

flightPhase runOnPad(uint32_t tick);
flightPhase runAscending(uint32_t tick);
flightPhase runDescending(uint32_t tick);
flightPhase runPostFlight(uint32_t tick);

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t tick = 0;
  static flightPhase status = ONPAD;
  uint32_t tickTime = getTickTime(status);
  bool x = true;
  (void) x;
  if(status == POST_FLIGHT){
    while(millis() - lastTime < tickTime/1000){
      //waste time
      x = x;
    }
    lastTime = millis();
  } else {
    while(micros() - lastTime < tickTime){
      //waste time
      x = x;
    }
    lastTime = micros();
  }

  switch(status){
  case ONPAD:
    status = runOnPad(tick);
    break;
  case ASCENDING:
    status = runAscending(tick);
    break;
  case DESCENDING:
    status = runDescending(tick);
    break;
  case POST_FLIGHT:
  default:
    status = runPostFlight(tick);
  }
  tick++;
}

flightPhase runOnPad(uint32_t tick){
  //sample sensors
  static bmpReading lastBmp;
  static gpsReading lastGps;
  
  imuReading imuSample = imu.sample(); //sample IMU first to maximize consistency

  bool hasLaunched = detectLaunch(imuSample.accel); //might need to calibrate accel data before feeding to this function
  if(tick % 5 == 0){
    lastBmp = getBMP();
    lastBmp.state = 0; //this corresponds to ONPAD
    detectApogee(imuSample.accel, lastBmp.altitude, hasLaunched); //need to run this prelaunch to get calibrations
    recordData(lastBmp, true);
  }

  attitude.calibrateGyro(imuSample.gyro);
  attitude.updateAttitude(imuSample.gyro, false, imuSample.time);

  imuSample.attitude = attitude.getCurrentAttitude();
  recordData(imuSample, true);

  if(tick % 100 == 2 && lastGps.longitude == 0){//1 reading then use cached value
    //TODO: if there's no GPS this could be a problem
    lastGps = getGPS();
    recordData(lastGps, true);
  }
  if(tick % 50 == 1){//twice per second with a slight offset to avoid overlapping with bmp
    transmitData(lastBmp.altitude, lastGps, '0');
  }
  if(tick % 1000 == 3 && tick < 30000){
    XBeeSerial.print("Calibrating IMU... time remaining: ");
    XBeeSerial.print((30000 - tick)/6000);
    XBeeSerial.print(":");
    int seconds = ((30000 - tick)/100) % 60;
    if(seconds < 10)
      XBeeSerial.print("0");
    XBeeSerial.println(seconds);
  }
  if(hasLaunched)
    return ASCENDING;
  return ONPAD;
}

flightPhase runAscending(uint32_t tick){ //this will run similarly to ONPAD except hasLaunched will be true
  //sample sensors
  static bmpReading lastBmp;
  static gpsReading lastGps;
  bool apogeeReached = false;
  imuReading imuSample = imu.sample(); //sample IMU first to maximize consistency
  
  if(tick % 5 == 0){
    lastBmp = getBMP();
    lastBmp.state = 1; //this corresponds to ASCENDING
    apogeeReached = detectApogee(imuSample.accel, lastBmp.altitude, true);
    recordData(lastBmp, false);
  }
  attitude.updateAttitude(imuSample.gyro, true, imuSample.time);
  imuSample.attitude = attitude.getCurrentAttitude();
  recordData(imuSample, false);

  if(tick % 5 == 1){//20 times per second with a slight offset to avoid overlapping with bmp and gps
    transmitData(lastBmp.altitude, lastGps, '1');
  }
  if(apogeeReached)
    return DESCENDING;
  return ASCENDING;
}

flightPhase runDescending(uint32_t tick){//this runs at 20hz
  static gpsReading lastGps;
  static double lastAlt = getBMP().altitude + 5;//initialize so that landing detection isn't triggered accidentally
  static bool initialDump = false;
  static bool deployedMain = false;

  if(!initialDump) {
    initialDump = true;
    backupToSD();
  }

  //sample sensors
  bmpReading bmpSample = getBMP();
  bmpSample.state = 2; //this corresponds to DESCENDING
  recordData(bmpSample, false);

  //TODO: or vertical velocity above threshold (50m/s?)
  static double altForVelocity = bmpSample.altitude;
  double currentVelocity = (altForVelocity - bmpSample.altitude) * 20; //delta alt divided by dt (.05)

  if ((bmpSample.altitude < MAIN_ALTITUDE || currentVelocity > 50) && !deployedMain) {
    digitalWrite(PYRO0_PIN, HIGH);
    delay(1000);
    digitalWrite(PYRO0_PIN, LOW);
    digitalWrite(PYRO1_PIN, HIGH);
    delay(1000);
    digitalWrite(PYRO1_PIN, LOW);
    deployedMain = true;
  }

  altForVelocity = bmpSample.altitude;



  if(tick % 20 == 1){//still one time per second
    lastGps = getGPS();
    recordData(lastGps, false);
  }
  if(tick % 100 == 2){//backup to SD every 5 seconds
    backupToSD();
  }
  transmitData(bmpSample.altitude, lastGps, '2');

  if(tick % 100 == 3){//every 5 seconds check if we are still descending
    if(lastAlt - bmpSample.altitude < 1){//if altitude hasn't changed more than 1 meter in 5 seconds, we're on the ground
      return POST_FLIGHT;
    }
    lastAlt = bmpSample.altitude;
  }
  return DESCENDING;
}

flightPhase runPostFlight(uint32_t tick){//this runs at 1hz
  static bool initialDump = false;

  if(!initialDump) {
    initialDump = true;
    backupToSD();
  }
  //once we're on the ground we can stop recording and start just broadcasting GPS somewhat infrequently
  static bmpReading bmpSample = getBMP(); //no need to add a state to this sample since it won't be recorded
  if(tick % 5 == 0){//broadcast every 5 seconds
    gpsReading gpsSample = getGPS();
    transmitData(bmpSample.altitude, gpsSample, '3');
  }
  return POST_FLIGHT;
}
