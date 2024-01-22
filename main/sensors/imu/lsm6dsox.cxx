/*****************************************************************************
 * This file is for all imu queries from the LSM6DSOX
 * Calculations such as attitude determination should go in a separate file
 * 
 * NOTE: Due to the current implementation this library, each flight computer
 * can only have 1 LSM6DSOX IMU.
 ****************************************************************************/
#ifdef LSM6DSOX

#include "imu.h"

#include <Adafruit_LSM6DSOX.h>

Adafruit_LSM6DSOX sox;

/**
 * @brief Sample the IMU (Gyroscope and Accelerometer)
 * 
 * @return imuReading 
 */
imuReading imu::sample() {
    imuReading imuSample;
    imuSample.time = micros(); //current timestamp

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    sox.getEvent(&accel, &gyro, &temp);

    // convert acceleration data from m/s^2 to Gs
    // TODO: correct axis (Z is up, Y and X are arbitrary for now)
    imuSample.accel.x = accel.acceleration.x / 9.81;
    imuSample.accel.z = accel.acceleration.y / 9.81;
    imuSample.accel.y = accel.acceleration.z / 9.81;

    // TODO: correct axis (Z is up, Y and X are arbitrary for now)
    imuSample.gyro.x = gyro.gyro.x;
    imuSample.gyro.z = gyro.gyro.y;
    imuSample.gyro.y = gyro.gyro.z;

    (void) temp; //we can remove this if we decide to record temperature from the IMU

    return imuSample; //return gyro and accel data
}

void imu::setup() {
    XBeeSerial.print("Setting up IMU... ");

    if (!sox.begin_SPI(IMU_CS, SPI1_SCK, SPI1_MISO, SPI1_MOSI)) {
        while (1) {
            XBeeSerial.println("Failed to find LSM6DSOX chip, halting execution");
            delay(1000);
        }
    }

    sox.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
    sox.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
    sox.setAccelDataRate(LSM6DS_RATE_208_HZ);
    sox.setGyroDataRate(LSM6DS_RATE_208_HZ);

    XBeeSerial.println("Done");

}

#endif // LSM6DSOX