#include <LoRa.h>

#define RXD2 13
#define TXD2 14

void setup() {
  Serial.begin(115200);
  
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  LoRa.setPins(18, 23, 26);
  if (!LoRa.begin(868E6)) { // 868 MHz pour l'Europe
    Serial.println("Erreur de démarrage LoRa !");
    while (1);
  }
  LoRa.setTxPower(20);
  Serial.println("LILYGO prêt à recevoir les données du Mega sur GPIO13 et à émettre en LoRa.");
}

void loop() {
  if (Serial2.available() > 0) {
    String dustValue = Serial2.readStringUntil('\n');

    if (dustValue.length() > 0) {
      Serial.print("Reçu du Mega: '");
      Serial.print(dustValue);
      Serial.println("'");

      LoRa.beginPacket();
      LoRa.print("Poussiere: " + dustValue + " V");
      LoRa.endPacket();
      
      Serial.println("Donnée transmise via LoRa.");
    }
  }
}