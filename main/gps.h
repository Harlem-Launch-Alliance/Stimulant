// Test code for Ultimate GPS Using Hardware Serial.
// Need to remove the GPS.lastNMEA() and see if it still works, still get compilation error

#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial2

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

void setupGPS()
{
  Serial1.println("Setting up GPS...");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

gpsReading getGPS()
{
  gpsReading sample;

  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    c = GPS.lastNMEA();
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return sample; // we can fail to parse a sentence in which case we should just wait for another
  }

  if (GPS.fix) {
    sample.latitude = GPS.latitudeDegrees;
    sample.longitude = GPS.longitudeDegrees;
  }
  return sample;
}
