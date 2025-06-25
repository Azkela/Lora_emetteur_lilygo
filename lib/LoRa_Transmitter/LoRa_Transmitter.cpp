#include "LoRa_Transmitter.h"

LoRa_Transmitter::LoRa_Transmitter() {}

bool LoRa_Transmitter::begin(long frequency, int ss_pin, int rst_pin, int dio0_pin) {
  LoRa.setPins(ss_pin, rst_pin, dio0_pin);
  if (!LoRa.begin(frequency)) {
    return false;
  }
  LoRa.setTxPower(20);
  return true;
}

void LoRa_Transmitter::sendData(const char* groupID, float temp, float hum, float pres, double sound, float dust) {
  JsonDocument doc;
  doc["groupID"] = groupID;
  doc["temperature"] = serialized(String(temp, 2));
  doc["humidity"] = serialized(String(hum, 2));
  doc["pressure"] = serialized(String(pres, 2));
  doc["sound_db"] = serialized(String(sound, 2));
  doc["dust_v"] = serialized(String(dust, 3));

  String LoRaMessage;
  serializeJson(doc, LoRaMessage);

  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();

  Serial.print("Paquet LoRa envoyé : ");
  Serial.println(LoRaMessage);
}