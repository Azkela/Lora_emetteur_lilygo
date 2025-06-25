#ifndef DUST_READER_H
#define DUST_READER_H

#include <Arduino.h>

class Dust_Reader {
public:
  Dust_Reader();
  void begin(int rx_pin, int tx_pin, long baud_rate = 9600);
  bool checkForData(); // Renvoie true si de nouvelles données sont arrivées
  float getDustVoltage();

private:
  float dustVoltage;
};

#endif