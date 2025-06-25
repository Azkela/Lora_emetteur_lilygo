#ifndef LORA_TRANSMITTER_H
#define LORA_TRANSMITTER_H

#include <LoRa.h>
#include <ArduinoJson.h>

class LoRa_Transmitter {
public:
  LoRa_Transmitter();
  bool begin(long frequency, int ss_pin, int rst_pin, int dio0_pin);
  void sendData(const char* groupID, float temp, float hum, float pres, double sound, float dust);

private:
};

#endif