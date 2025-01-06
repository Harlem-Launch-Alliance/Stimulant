# Stimulant
Hardware agnostic flight software for high power rockets.

Want to contribute? Start [here](https://github.com/Harlem-Launch-Alliance/Stimulant/blob/main/docs/GETTING_STARTED.md).

# NOTE: At this time we are only accepting contributions from [HLA](hla.nyc) members for issues with the `beginner` label.

[Instructions for use](https://github.com/Harlem-Launch-Alliance/Stimulant#instructions-for-use)

## Goals:
| Feature | Status |
|---------|--------|
| launch detection | :heavy_check_mark: |
| altitude monitoring | :heavy_check_mark: |
| location monitoring | :heavy_check_mark: |
| attitude determination |  |
| velocity determination |  |
| postition determination (non-GPS) |  |
| live telemetry transmission | :heavy_check_mark: |
| apogee detection | :heavy_check_mark: |
| drogue chute deployment |  |
| main chute deployment |  |
| data acquisition (SD card) | :heavy_check_mark: |

## Software Architecture:
[Flow Chart](https://miro.com/app/board/uXjVPYUZ7mY=/?share_link_id=493963314311)

## Hardware:
This software can support any configuration of hardware with exactly one of each of the following types:
- Microcontroller
- Inertial Measurement Unit
- GPS
- Radio transceiver

The following hardware is currently supported:
- Teensy 4.1
- RaspberryPi Pico
- Adafruit BMP388/BMP390 Barometric Altimeter
- GY521 IMU (MPU 6050)
- LSM6DSOX IMU
- Adafruit Ultimate GPS
- Xbee-PRO S3B

## Best Practices For Making Changes:
- make sure your code compiles to verify your changes
- comment your code
- multiple small functions are better than one big one. Remember, each function should only do one thing
- break out groups of functions into their own files. Long files are difficult to read. Ideally `main.ino` would only have `void setup()` and `void loop()`

## Instructions for use:
1. Connect all sensors and power source.
2. If using pyros, verify that the the power supply is OFF and then attach them.
2. Integrate the flight computer with rocket (try to keep IMU aligned with axis of rotation)
3. Place rocket on launch rail.
4. Power on flight computer
    * **VERIFY BEFOREHAND**: 1) Make sure rocket is upright on the launch pad. 2) Make sure Pyros have been disconnected 
    * **CAUTION**: Once powered on, any sudden movements may trigger launch detection
5. Wait at least 5 minutes between powering on and launching to allow for proper sensor calibration
6. Launch!

Note: Due to the use of the `micros()` function, the rocket MUST be launched within 60 minutes of powering on. If you do not launch within that period you must restart the flight computer by turning it off and then on again.
