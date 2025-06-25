#ifndef INMP441_MIC_H
#define INMP441_MIC_H

#include <Arduino.h>
#include <driver/i2s.h>

class INMP441_Mic {
public:
  INMP441_Mic();
  bool begin(int ws_pin, int sd_pin, int sck_pin);
  void read();
  double getSoundLevelDb();

private:
  double soundLevelDb;
  i2s_port_t i2s_port = I2S_NUM_0;
};

#endif