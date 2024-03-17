#include "./lib/mock_arduino.h"

void delay(int millis) {
    (void)millis;
}

unsigned long micros() {
    static unsigned long micros = 1;
    micros += 10;
    return micros;
}

uint8_t MockTwoWire::read() {
    return 0b1000; // 8
}

void MockTwoWire::write(int message) {
    (void)message;
}

void MockTwoWire::beginTransmission(int address) {
    (void)address;
}

void MockTwoWire::endTransmission(bool torf = false) {
    (void)torf;
}

void MockTwoWire::requestFrom(int address, int style, bool idk) {
    (void)address;
    (void)style;
    (void)idk;
}

MockTwoWire Wire;

MockSerial Serial1;
