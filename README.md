# Catalyst 2 Flight Computer
Designed for Teensy 4.1

Want to contribute? Start [here](https://github.com/Harlem-Launch-Alliance/blob/main/GETTING_STARTED.md).

[Instructions for use](https://github.com/Harlem-Launch-Alliance/Catalyst-2#instructions-for-use)

## Goals:
- launch detection
- altitude monitoring
- attitude, velocity, and position determination
- live telemetry transmission
- automated apogee detection and parachute deployment(s)

## Hardware:
- Teensy 4.1
- Adafruit BMP388 Barometric Altimeter
- GY521 IMU (MPU 6050)
- Adafruit Ultimate GPS
- Xbee-PRO S3B
- 5V buzzer

## Dependencies:
- https://github.com/jonnieZG/EWMA
- https://github.com/adafruit/Adafruit_BMP3XX
- https://github.com/adafruit/Adafruit_GPS

## Best Practices For Making Changes:
- make sure your code compiles to verify your changes
- comment your code
- if you are including a new library in your code, add the GitHub link to the dependencies list above
- multiple small functions are better than one big one. Remember, each function should only do one thing
- break out groups of functions into their own files. Long files are difficult to read. Ideally `main.ino` would only have `void setup()` and `void loop()`

## Instructions for use:
1. Connect all sensors and power source
2. Integrate Teensy with rocket (try to keep IMU aligned with axis of rotation)
3. Put rocket on launch rail
4. Power on Teensy
    * **CAUTION**: Once powered on, any sudden movements may trigger launch detection
5. Wait at least 5 minutes between powering on and launching to allow for proper sensor calibration
6. Launch!

Note: Due to the use of the `micros()` function, the rocket MUST be launched within 60 minutes of powering on. If you do not launch within that period you must restart the flight computer by turning it off and then on again.
