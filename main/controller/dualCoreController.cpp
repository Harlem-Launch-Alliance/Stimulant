/*****************************************************************************************
 * 
 * Main functions for a dual core system
 * 
*****************************************************************************************/

#ifdef DUAL_CORE

#include "apogee.h"
#include "controller.h"
#include "data.h"
#include "sensors/altimeter/bmp3xx.h"
#include "sensors/gps/adafruit_gps.h"
#include "utils/utils.h"

/// @brief waste time to keep the loop running at a consistent rate
///
/// @param state current phase of flight
static void wasteTime(FlightPhase state){
    static uint32_t lastTime = 0;
    uint32_t tickTime = getTickTime(state);
    if(state == POST_FLIGHT){
        while(millis() - lastTime < tickTime/1000){
            //waste time
        }
        lastTime = millis();
    } else {
        while(micros() - lastTime < tickTime){
            //waste time
        }
        lastTime = micros();
    }
}

RocketController::RocketController() : state(ONPAD), tick(0) {}

void RocketController::setup(){
    delay(500); // give sensors time to boot up

    // set up radio
    XBeeSerial.begin(115200);      // xBee baudrate: 115200, 9600
    XBeeSerial.print("\n\n\n");

    pinMode(BUZZER_PIN, OUTPUT);    // Set buzzer pin as an output

    setupPyros();

    for(int i = 0; i < 5; i++)  // Play 5 beeps
    {
    tone(BUZZER_PIN, TONE_HZ);  // Play a tone...
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

void RocketController::setup1() {
    // set up second core
}

void RocketController::loop(){
    wasteTime(state);
    switch(state){
        case ONPAD:
            state = runOnPad();
            break;
        case ASCENDING:
            state = runAscending();
            break;
        case DESCENDING:
            state = runDescending();
            break;
        case POST_FLIGHT:
            state = runPostFlight();
            break;
    }
    tick++;
}

void RocketController::loop1() {
    // run second core
    // sample GPS...
    // broadcast telemetry...
    // backup to SD or flash...
    static bool FinalBackup = false;
    delay(1000);
    if (state == ASCENDING || state == DESCENDING) {
        backupToSD();
    } else if (state == POST_FLIGHT && !FinalBackup) {
        backupToSD();
        FinalBackup = true;
    }
}

FlightPhase RocketController::runOnPad(){
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

FlightPhase RocketController::runAscending() {
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

FlightPhase RocketController::runDescending() {
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

    static double altForVelocity = bmpSample.altitude;
    double currentVelocity = (altForVelocity - bmpSample.altitude) * 20; //delta alt divided by dt (.05)

    if ((bmpSample.altitude < MAIN_ALTITUDE || currentVelocity > 35) && !deployedMain) {
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

FlightPhase RocketController::runPostFlight() {
    //once we're on the ground we can stop recording and start just broadcasting GPS somewhat infrequently
    static bmpReading bmpSample = getBMP(); //no need to add a state to this sample since it won't be recorded
    if(tick % 5 == 0){//broadcast every 5 seconds
        gpsReading gpsSample = getGPS();
        transmitData(bmpSample.altitude, gpsSample, '3');
    }
    return POST_FLIGHT;
}

void RocketController::setupPyros(){
    pinMode(PYRO0_PIN, OUTPUT);     // Set pyro pins as an outputs
    pinMode(PYRO1_PIN, OUTPUT);
    digitalWrite(PYRO0_PIN, LOW);   // Set pyro pins to low
    digitalWrite(PYRO1_PIN, LOW);
}

#endif //DUAL_CORE
