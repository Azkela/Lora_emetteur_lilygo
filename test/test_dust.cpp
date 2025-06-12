#include <Arduino.h>

#define ILED_PIN 21      // Pin de contrôle LED IR
#define VO_PIN 35         // ADC pin pour lecture analogique Vo

void setup() {
  Serial.begin(115200);
  pinMode(ILED_PIN, OUTPUT);
  digitalWrite(ILED_PIN, LOW);
  analogReadResolution(12);  // ADC 12 bits
}

void loop() {
  digitalWrite(ILED_PIN, HIGH);       // Allume LED IR
  delayMicroseconds(320);              // Maintenir 0.32 ms
  //delayMicroseconds(280);              // Attente stabilisation 0.28 ms (peut être ajustée)
  int adcValue = analogRead(VO_PIN);  // Lire ADC 12 bits (0-4095)
  digitalWrite(ILED_PIN, LOW);        // Eteindre LED IR

  // Conversion voltage (3.3V / 4095)
  float voltage = adcValue * (3.3 / 4095.0);

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print("  Voltage: ");
  Serial.println(voltage, 3);

  delay(10); // Pause totale ~10 ms entre lectures
}
