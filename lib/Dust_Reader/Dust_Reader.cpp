#include "Dust_Reader.h"

Dust_Reader::Dust_Reader() : dustVoltage(0.0) {}

void Dust_Reader::begin(int rx_pin, int tx_pin, long baud_rate) {
  Serial2.begin(baud_rate, SERIAL_8N1, rx_pin, tx_pin);
}

bool Dust_Reader::checkForData() {
  if (Serial2.available() > 0) {
    String dustValueStr = Serial2.readStringUntil('\n');
    dustValueStr.trim();
    if (dustValueStr.length() > 0) {
      dustVoltage = dustValueStr.toFloat();
      return true; 
    }
  }
  return false; 
}

float Dust_Reader::getDustVoltage() { return dustVoltage; }