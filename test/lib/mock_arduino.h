#pragma once

#include <string>

#define PI 3.1415926535897932384626433832795
#define BMP3_NO_OVERSAMPLING 1
#define BMP3_OVERSAMPLING_2X 1
#define BMP3_IIR_FILTER_COEFF_3 1
#define BMP3_ODR_100_HZ 1

class Adafruit_BMP3XX
{
public:
    bool performReading() {
        pressure = 100;
        temperature = 1;
        return true;
    }

    double readAltitude(double groundPressure) {
        (void)groundPressure;
        return 1;
    }

    bool begin_SPI(int chipSelect) {
        (void)chipSelect;
        return true;
    }

    void setTemperatureOversampling(int setting) {
        (void)setting;
    }

    void setPressureOversampling(int setting) {
        (void)setting;
    }
    
    void setIIRFilterCoeff(int setting) {
        (void)setting;
    }

    void setOutputDataRate(int setting) {
        (void)setting;
    }

    double pressure;
    double temperature;

};

unsigned long micros() {
    static unsigned long micros = 1;
    micros += 10;
    return micros;
}

class Serial
{
public:
    void print(std::string s) {
        (void)s;
    }

    void println(std::string s) {
        (void)s;
    }
};

Serial Serial1;

void delay(int millis) {
    (void)millis;
}