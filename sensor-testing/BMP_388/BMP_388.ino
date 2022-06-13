/***************************************************************************
  Modified code from Limor Fried & Kevin Townsend for Adafruit Industries.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

//#define BMP_SCK 13
//#define BMP_MISO 12
//#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

double Altitude, temperature, pressure, altOffset = 0;

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  setupBMP();
  
  // calculating the initial altitude on the ground to use as offset
  double total = 0;
  for(int i = 0; i < 100; i++)
  {
    getBMP();
    total += Altitude;
  }
  altOffset = total/100.0;
}

void loop() {
  getBMP();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Offset = ");
  Serial.print(altOffset);
  Serial.println(" m");

  Serial.print("Altitude = ");
  Serial.print(Altitude);
  Serial.println(" m");

  delay(100);
}

void setupBMP()
{
  //if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  for(int i = 0; i < 100; i++)                             // runs BMP 100 times to flush out data
    getBMP();
}

void getBMP()
{
  if (! bmp.performReading()) {
    Serial.println("Failed to perform BMP388 reading.");
    return;
  }
  temperature = bmp.temperature;          // Temperature in Celcius
  pressure = bmp.pressure / 100.0;        // Pressure in hPa
  Altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - altOffset; // Approximate altitude in meters
}
