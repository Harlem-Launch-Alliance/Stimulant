// This code is modified from the example GPS_HardwareSerial_Parsing
// The timer in the loop needs to be fixed before testing again.

// Test code for Ultimate GPS Using Hardware Serial
// This code shows how to listen to the GPS module via polling.

#include <Adafruit_GPS.h>

#define GPSSerial Serial2     // Name of the hardware serial port
Adafruit_GPS GPS(&GPSSerial); // Connects to the GPS on the hardware port

void setupGPS()
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // Set the update rate:
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  GPS.sendCommand(PGCMD_ANTENNA);       // Request updates on antenna status, comment out to keep quiet
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);    // Ask for firmware version
}

//void loop() // run over and over again
//{
//  // read data from the GPS in the 'main loop'
//  char c = GPS.read();
//  // if you want to debug, this is a good time to do it!
//
//  // if a sentence is received, we can check the checksum, parse it...
//  if (GPS.newNMEAreceived()) {
//    // a tricky thing here is if we print the NMEA sentence, or data
//    // we end up not listening and catching other sentences!
//    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
//    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
//    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
//      return; // we can fail to parse a sentence in which case we should just wait for another
//  }

//  // approximately every 2 seconds or so, print out the current status
//  uint32_t start_time = millis();
//  if (millis() - start_time > 2000) {
//    if (GPS.fix) {
//      Serial.print("Location: ");
//      Serial.print(GPS.latitudeDegrees, 8);
//      Serial.print(", ");
//      Serial.println(GPS.longitudeDegrees, 8);
//      Serial.print("Altitude: "); Serial.println(GPS.altitude);
//      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
//    }
//    else
//      Serial.println("No Fix");
//  }
}
