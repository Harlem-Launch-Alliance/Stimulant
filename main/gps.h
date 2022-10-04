// Test code for Ultimate GPS Using Hardware Serial.
// Need to remove the GPS.lastNMEA() and see if it still works, still get compilation error

#include <Adafruit_GPS.h>
#include "utils.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial2
#define ERR_NO_GPS "flightData-noDate"

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

String setupGPS()
{
  Serial1.println("Setting up GPS...");
  unsigned int startTime = millis();
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);                   // 9600 is the default baud rate
  GPS.sendCommand("$PMTK251,38400*27");             //set baud rate to 38400
  GPSSerial.end();
  delay(1000);
  GPS.begin(38400);            // set serial to 38400
  delay(1000);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

  unsigned int timer = 0;
  
  while(!GPS.fix || GPS.year == 0){
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPS.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      c = *GPS.lastNMEA(); //unclear what this statement does. Variable is unused atm
      (void) c;
      if (!GPS.parse(GPS.lastNMEA())){} // this also sets the newNMEAreceived() flag to false
    }
    if(timer < millis()){
      timer = millis() + 10000;
      int minutes = (timer - startTime)/60000;
      int secs = ((timer - startTime) / 1000) % 60;
      Serial1.print("Acquiring GPS signal... Time elapsed: ");
      Serial1.print(minutes);//minutes
      Serial1.print(":");
      if(secs < 10)
        Serial1.print("0");
      Serial1.println(secs);//seconds
    }
    if(millis() - startTime > GPS_WAIT_TIME * 1000){
      Serial1.println("CAUTION!!! GPS timeout: Progressing with no GPS signal");
      return ERR_NO_GPS;
    }
  }
  Serial1.println("GPS signal acquired");
  uint8_t hour = GPS.hour;
  uint8_t minute = GPS.minute;
  uint8_t year = GPS.year;
  uint8_t month = GPS.month;
  uint8_t day = GPS.day;
  uint8_t yearPrefix = 20;
  
  String date = String(yearPrefix).concat(year).concat('-').concat(month).concat('-').concat(day).concat("--").concat(hour).concat('-').concat(minute);
  Serial1.print("Date: ");
  Serial1.println(date);
  return date;
}

gpsReading getGPS()
{
  unsigned long t1 = micros();
  while(micros() - t1 < 100000){//give GPS 100 ms to deliver a sentence
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPS.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      c = *GPS.lastNMEA(); //unclear what this statement does. Variable is unused atm
      (void) c;
      if (!GPS.parse(GPS.lastNMEA())){} // this also sets the newNMEAreceived() flag to false
    }
  }

  gpsReading sample;
  
  if(GPS.fix){
    sample.latitude = GPS.latitudeDegrees;
    sample.longitude = GPS.longitudeDegrees;
  }
  sample.time = millis();
  return sample;
}
