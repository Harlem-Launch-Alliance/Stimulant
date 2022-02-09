/*************************************************************************************
                                 Harlem Launch Alliance
                            Recovery Systems Group 2021 - 2022
                              Catalyst 2.1 Flight Computer
 *************************************************************************************/
#include <Wire.h>             // to communicate with I2C devices
#include <SD.h>               // for data storage on the onboard microSD card
#include "Adafruit_BMP3XX.h"  // BMP388 library

// Constants:
#define SEALEVELPRESSURE_HPA (1013.25)
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 33;

// Objects:
Adafruit_BMP3XX bmp;
File dataFile;

// Variables:
double Altitude, temperature, pressure, altOffset = 0;    // final values
float Time;
bool apogeeReached;

void setupBMP()
{
  Serial1.print("Setting up BMP388: ");
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial1.println("could not find a valid BMP388 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  for(int i = 0; i < 100; i++)                             // runs BMP 100 times to flush out data
    getBMP();
  Serial1.println("successful");
}

void getBMP()
{
  if (! bmp.performReading()) {
    Serial1.println("Failed to perform BMP388 reading.");
    return;
  }
  temperature = bmp.temperature;          // Temperature in Celcius
  pressure = bmp.pressure / 100.0;        // Pressure in hPa
  Altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - altOffset; // Approximate altitude in meters
}

void transmitData()
{
  Serial1.print(Time); Serial1.print("\t");
  Serial1.print(tc); Serial1.print("\t");
  Serial1.print(temperature); Serial1.print("\t");
//  Serial1.print(pitch); Serial1.print("\t");
//  Serial1.print(roll); Serial1.print("\t");
//  Serial1.print(Ax); Serial1.print("\t");
//  Serial1.print(Ay); Serial1.print("\t");
//  Serial1.print(Az); Serial1.print("\t");
//  Serial1.print(Gx); Serial1.print("\t");
//  Serial1.print(Gy); Serial1.print("\t");
//  Serial1.print(Gz); Serial1.print("\t");
  Serial1.print(pressure); Serial1.print("\t"); Serial1.print("\t");    
  Serial1.print(Altitude); Serial1.print("\t");
  Serial1.println(apogeeReached);
}
void recordData()
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(Time); dataFile.print("\t");
    dataFile.print(tc); dataFile.print("\t");
    dataFile.print(temperature); dataFile.print("\t");
//    dataFile.print(pitch); dataFile.print("\t");
//    dataFile.print(roll); dataFile.print("\t");
//    dataFile.print(Ax); dataFile.print("\t");
//    dataFile.print(Ay); dataFile.print("\t");
//    dataFile.print(Az); dataFile.print("\t");
//    dataFile.print(Gx); dataFile.print("\t");
//    dataFile.print(Gy); dataFile.print("\t");
//    dataFile.print(Gz); dataFile.print("\t");
    dataFile.print(pressure); dataFile.print("\t"); dataFile.print("\t");
    dataFile.print(Altitude); dataFile.print("\t");
    dataFile.println(apogeeReached);
    dataFile.close();                       // closing file is very important
  }
  else
    Serial1.println("error opening datalog.txt");
}

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
  
  // calculating the initial altitude on the ground to use as offset
  double total = 0;
  for(int i = 0; i < 100; i++)
  {
    getBMP();
    total += Altitude;
  }
  altOffset = total/100.0;
  
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Catalyst 2 Flight Computer Test December 14th, 2021");
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
  Time = float(millis()) / 1000.0;
  getBMP();
  apogeeReached = detectApogee(Ax, Ay, Az, Altitude);
  recordData();
  transmitData();
}
