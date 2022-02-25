# Catalyst 2 Flight Computer
Designed for Teensy 4.1

## Goals:
- altitude monitoring
- attitude, velocity, and position determination
- live telemetry transmission
- automated apogee detection and parachute deployment(s)

## Hardware:
- Adafruit BMP388 Barometric Altimeter
- GY521 IMU

## Dependencies:
- https://github.com/ivanseidel/LinkedList
- https://github.com/jonnieZG/EWMA
- https://github.com/adafruit/Adafruit_BMP3XX
- https://github.com/adafruit/Adafruit_GPS

## Best Practices For Making Changes:
- make sure your code compiles to verify your changes
- comment your code
- if you are including a new library in your code, add the GitHub link to the dependencies list above
- multiple small functions are better than one big one. Remember, each function should only do one thing
- breakout groups of functions into their own files. Long files are difficult to read. Ideally `main.ino` would only have `void setup()` and `void loop()`
