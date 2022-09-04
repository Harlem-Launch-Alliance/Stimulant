/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/
#include "ringQueue.h"

const int chipSelect = BUILTIN_SDCARD;
char filename[50];
RingQueue<imuReading> imuQueue;
RingQueue<bmpReading> bmpQueue;
RingQueue<gpsReading> gpsQueue;

void transmitData(double altitude, gpsReading gps, char phase)
{
  Serial1.print(phase); //0: ONPAD, 1: ASCENDING, 2: DESCENDING, 3: POST_FLIGHT
  Serial1.print(" ");
  Serial1.print(gps.latitude, 4);
  Serial1.print(" ");
  Serial1.print(gps.longitude, 4);
  Serial1.print(" ");
  Serial1.println(altitude,1);
}

void setupSD(String date){
  if (!SD.begin(chipSelect)){
    Serial1.println("MicroSD card: failed or not present");
    return;
  }
  else
    Serial1.println("MicroSD card: successful");
  String filestart = "Catalyst2-";
  filestart.concat(date).concat(".csv");
  filestart.toCharArray(filename, 50);
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time(us),Altitude(m),Latitude,Longitude,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ,AttitudeX,AttitudeY,AttitudeZ,state");
    dataFile.close();
  }
  else {
    Serial1.println("MicroSD card: error opening file");
  }
}

void recordData(imuReading sample, bool prelaunch){
  imuQueue.enqueue(sample);
  if(prelaunch){
    while(!imuQueue.isEmpty() && imuQueue.peek().time < millis() - 2000){
      imuQueue.dequeue();
    }
  }
}

void recordData(bmpReading sample, bool prelaunch){
  bmpQueue.enqueue(sample);
  if(prelaunch){
    while(!bmpQueue.isEmpty() && bmpQueue.peek().time < millis() - 2000){
      bmpQueue.dequeue();
    }
  }
}

void recordData(gpsReading sample, bool prelaunch){
  gpsQueue.enqueue(sample);
  if(prelaunch){
    while(!gpsQueue.isEmpty() && gpsQueue.peek().time < millis() - 2000){
      gpsQueue.dequeue();
    }
  }
}

void backupToSD(){
  File dataFile = SD.open(filename, FILE_WRITE);
  if(!dataFile){
    Serial1.println("error opening datalog");
    return;
  }
  while(!imuQueue.isEmpty()){
    double altitude = 0;
    double latitude = 0;
    double longitude = 0;
    imuReading imuSample = imuQueue.peek();
    imuQueue.dequeue();
    if(!bmpQueue.isEmpty()){
      bmpReading bmpSample = bmpQueue.peek();
      if(bmpSample.time < imuSample.time){
        altitude = bmpSample.altitude;
        bmpQueue.dequeue();
      }
    }
    if(!gpsQueue.isEmpty()){
      gpsReading gpsSample = gpsQueue.peek();
      if(gpsSample.time < imuSample.time){
        longitude = gpsSample.longitude;
        latitude = gpsSample.latitude;
        gpsQueue.dequeue();
      }
    }
    dataFile.print(imuSample.time); dataFile.print(',');
    if(altitude != 0)
      dataFile.print(altitude, 1);
    dataFile.print(',');
    if(latitude != 0)
      dataFile.print(latitude, 4);
    dataFile.print(',');
    if(longitude != 0)
      dataFile.print(longitude, 4);
    dataFile.print(',');
    dataFile.print(imuSample.gyro.x); dataFile.print(',');
    dataFile.print(imuSample.gyro.y); dataFile.print(',');
    dataFile.print(imuSample.gyro.z); dataFile.print(',');
    dataFile.print(imuSample.accel.x); dataFile.print(',');
    dataFile.print(imuSample.accel.y); dataFile.print(',');
    dataFile.print(imuSample.accel.z); dataFile.print(',');
    dataFile.print(imuSample.attitude.x); dataFile.print(',');
    dataFile.print(imuSample.attitude.y); dataFile.print(',');
    dataFile.print(imuSample.attitude.z); dataFile.println(',');
    //TODO add state
  }
  while(!bmpQueue.isEmpty()){
    double latitude = 0;
    double longitude = 0;
    bmpReading bmpSample = bmpQueue.peek();
    bmpQueue.dequeue();
    if(!gpsQueue.isEmpty()){
      gpsReading gpsSample = gpsQueue.peek();
      if(gpsSample.time < bmpSample.time){
        longitude = gpsSample.longitude;
        latitude = gpsSample.latitude;
        gpsQueue.dequeue();
      }
    }
    dataFile.print(bmpSample.time); dataFile.print(',');
    dataFile.print(bmpSample.altitude, 1); dataFile.print(',');
    if(latitude != 0)
      dataFile.print(latitude, 4);
    dataFile.print(',');
    if(longitude != 0)
      dataFile.print(longitude, 4);
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.println(',');
  }
  dataFile.close(); 
}
