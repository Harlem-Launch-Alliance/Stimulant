/*****************************************************************************
 * This file is for saving and transmitting data
 * currently using an SD card and Xbee respectively
 ****************************************************************************/
#include "data.h"

#include <SD.h>

const int CS_SD = BUILTIN_SDCARD;

void transmitData(double altitude, gpsReading gps, char phase)
{
  //phase packet
  XBeeSerial.print("0 ");
  XBeeSerial.println(phase); //0: ONPAD, 1: ASCENDING, 2: DESCENDING, 3: POST_FLIGHT

  //altitude packet
  XBeeSerial.print("1 ");
  XBeeSerial.println(altitude,1);

  //gps packet
  XBeeSerial.print("3 ");
  XBeeSerial.print(gps.latitude, 4);
  XBeeSerial.print(" ");
  XBeeSerial.println(gps.longitude, 4);
}

SDCard::SDCard() {
  //mutex_init(&m_mutex);
}

void SDCard::setup(String date){
  //mutex_enter_blocking(&m_mutex);
  if (!SD.begin(CS_SD)){
    XBeeSerial.println("MicroSD card: failed or not present");
    //mutex_exit(&m_mutex);
    return;
  }
  else
    XBeeSerial.println("MicroSD card: successful");
  String filestart = "Stimulant-";
  filestart.concat(date);
  filestart.concat(".csv");
  filestart.toCharArray(filename, 50);
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time(us),Altitude(m),Latitude,Longitude,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ,AttitudeX,AttitudeY,AttitudeZ,state");
    dataFile.close();
  }
  else {
    XBeeSerial.println("MicroSD card: error opening file");
    XBeeSerial.print("Filename: ");
    XBeeSerial.println(filename);
    delay(5000);
  }
  //mutex_exit(&m_mutex);
}

void SDCard::record(imuReading sample, bool prelaunch){
  //mutex_enter_blocking(&m_mutex);
  imuQueue.enqueue(sample);
  if(prelaunch){
    while(!imuQueue.isEmpty() && imuQueue.peek().time < micros() - 2000000){
      imuQueue.dequeue();
    }
  }
  //mutex_exit(&m_mutex);
}

void SDCard::record(bmpReading sample, bool prelaunch){
  //mutex_enter_blocking(&m_mutex);
  bmpQueue.enqueue(sample);
  if(prelaunch){
    while(!bmpQueue.isEmpty() && bmpQueue.peek().time < micros() - 2000000){
      bmpQueue.dequeue();
    }
  }
  //mutex_exit(&m_mutex);
}

void SDCard::record(gpsReading sample, bool prelaunch){
  //mutex_enter_blocking(&m_mutex);
  gpsQueue.enqueue(sample);
  if(prelaunch){
    while(!gpsQueue.isEmpty() && gpsQueue.peek().time < micros() - 2000000){
      gpsQueue.dequeue();
    }
  }
  //mutex_exit(&m_mutex);
}

void SDCard::backup(){
  XBeeSerial.println("backing up...");
  File dataFile = SD.open(filename, FILE_WRITE);
  char buffer[50];
  if(!dataFile){
    XBeeSerial.println("error opening datalog");
    return;
  }
  //mutex_enter_blocking(&m_mutex);
  XBeeSerial.println("backing up imu");
  while(!imuQueue.isEmpty()){
    XBeeSerial.println(".");
    //mutex_exit(&m_mutex);
    double altitude = 0;
    double latitude = 0;
    double longitude = 0;
    int state = -1; //initialize with invalid state so that it's clear if something isn't working right
    //mutex_enter_blocking(&m_mutex);
    imuReading imuSample = imuQueue.peek();
    imuQueue.dequeue();
    if(!bmpQueue.isEmpty()){
      bmpReading bmpSample = bmpQueue.peek();
      if(bmpSample.time < imuSample.time){
        altitude = bmpSample.altitude;
        state = bmpSample.state;
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
    //mutex_exit(&m_mutex);
    itoa(imuSample.time,buffer,10);
    dataFile.print(buffer); dataFile.print(',');
    if(altitude != 0) {
      dtostrf(altitude, 6, 1, buffer);
      dataFile.print(buffer);
    }
    dataFile.print(',');
    if(latitude != 0) {
      dtostrf(latitude, 6, 4, buffer);
      dataFile.print(buffer);
    }
    dataFile.print(',');
    if(longitude != 0) {
      dtostrf(longitude, 6, 4, buffer);
      dataFile.print(buffer);
    }
    dataFile.print(',');
    dtostrf(imuSample.gyro.x, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.gyro.y, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.gyro.z, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.accel.x, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.accel.y, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.accel.z, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.attitude.x, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.attitude.y, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(imuSample.attitude.z, 8, 4, buffer);
    dataFile.print(buffer); dataFile.print(',');
    if(altitude != 0) {
      itoa(state,buffer,10);
      dataFile.print(buffer);
    }
    dataFile.println(',');
    //mutex_enter_blocking(&m_mutex);
  }
  XBeeSerial.println("backing up bmp");
  while(!bmpQueue.isEmpty()){
    XBeeSerial.println(".");
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
    //mutex_exit(&m_mutex);
    itoa(bmpSample.time,buffer,10);
    dataFile.print(buffer); dataFile.print(',');
    dtostrf(bmpSample.altitude, 6, 1, buffer);
    dataFile.print(buffer); dataFile.print(',');
    if(latitude != 0) {
      dtostrf(latitude, 6, 4, buffer);
      dataFile.print(buffer);
    }
    dataFile.print(',');
    if(longitude != 0) {
      dtostrf(longitude, 6, 4, buffer);
      dataFile.print(buffer);
    }
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    dataFile.print(',');
    itoa(bmpSample.state,buffer,10);
    dataFile.print(buffer);
    dataFile.println(',');
    //mutex_enter_blocking(&m_mutex);
  }
  //mutex_exit(&m_mutex);
  dataFile.close();
  XBeeSerial.println("back up complete");
}
