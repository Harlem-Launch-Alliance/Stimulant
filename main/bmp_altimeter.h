/*****************************************************************************
 * This file is for all barometric altimeter queries
 * 
 ****************************************************************************/


Adafruit_BMP3XX bmp; //initialize sensor

#define SEALEVELPRESSURE_HPA (1013.25)

bmpReading getBMP()
{
  bmpReading sample;
  if (!bmp.performReading()) {
    Serial1.println("Failed to perform BMP388 reading.");
    return sample;
  }
  sample.temp = bmp.temperature;          // Temperature in Celcius
  sample.pressure = bmp.pressure / 100.0;        // Pressure in hPa
  sample.altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA)/* - altOffset*/; // Approximate altitude in meters
  sample.time = millis();
  return sample;
}

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
