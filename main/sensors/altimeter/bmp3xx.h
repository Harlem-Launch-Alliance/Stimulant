/*****************************************************************************
 * This file is for all barometric altimeter queries
 * 
 ****************************************************************************/
#pragma once

#ifndef UNIT_TEST
#include "Adafruit_BMP3XX.h"  // BMP388 library
#endif // UNIT_TEST
#include "../../utils/datatypes.h"

double GNDLEVELPRESSURE_HPA;

Adafruit_BMP3XX bmp; //initialize sensor

/**
 * @brief Sample Altimeter One Time
 * 
 * @note Using SPI because it is faster than I2C
 * 
 * @return bmpReading 
 */
bmpReading getBMP()
{
  bmpReading sample;
  if (!bmp.performReading()) {
    XBeeSerial.println("Failed to perform BMP388 reading.");
    return sample;
  }
  //sample.temp = bmp.temperature;          // Temperature in Celcius
  //sample.pressure = bmp.pressure / 100.0;        // Pressure in hPa
  sample.altitude = bmp.readAltitude(GNDLEVELPRESSURE_HPA); // Approximate altitude in meters
  sample.time = micros();
  return sample;
}

void setGroundLevelPressure() { //Reads bmp data and sets ground lvl pressure
  double pressurePreFlight_hPa;
  double pressureSum = 0;  //Sum of 100 pressure readings
  for(int i = 0; i<100; i++){
    bmp.performReading();
    pressurePreFlight_hPa = bmp.pressure / 100.0; //sample pressure in hPa
    pressureSum += pressurePreFlight_hPa;
  }
  GNDLEVELPRESSURE_HPA = pressureSum / 100.0; //sets average ground lvl pressure
}

/**
 * @brief Establish Connection with Altimeter via SPI
 * 
 */
void setupBMP()
{
  XBeeSerial.print("Setting up BMP388... ");
  delay(3000);
  if (!bmp.begin_SPI(BMP_CS, SPI1_SCK, SPI1_MISO, SPI1_MOSI)) {
    XBeeSerial.println("could not find a valid BMP388 sensor, check wiring!");
    return;
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_100_HZ);
  for(int i = 0; i < 100; i++)                             // runs BMP 100 times to flush out data
    getBMP();
  bmpReading sample = getBMP();
  if(sample.altitude != 0)
    XBeeSerial.println("BMP setup successful.");
  else
    XBeeSerial.println("BMP setup failed, check wiring or pin settings.");
  delay(2000);
  setGroundLevelPressure();
}
