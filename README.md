# Catalyst 2 Flight Computer
Designed for Teensy 4.1

Want to contribute? Start [here](https://github.com/Harlem-Launch-Alliance/Catalyst-2/blob/main/GETTING_STARTED.md).

# NOTE: At this time we are only accepting contributions from [HLA](hla.nyc) members for issues with the `beginner` label.

[Instructions for use](https://github.com/Harlem-Launch-Alliance/Catalyst-2#instructions-for-use)

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
