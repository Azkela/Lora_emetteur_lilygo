#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <LoRa.h>

#define SS 18
#define RST 14
#define DI0 26
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BME280 bme;

const char* groupID = "MAR-2";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialisation LoRa
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Erreur de démarrage LoRa !");
    while (1);
  }

  // Initialisation du BME280
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!bme.begin(0x76)) { 
    Serial.println("Impossible de trouver le capteur BME280");
    while (1);
  }

  // Augmenter la puissance d'émission LoRa
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  Serial.println("LoRa et Capteurs Initialisés.");
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // Préparation des données à envoyer
  String data = "Temperature: " + String(temperature) + " C, "
                 + "Humidity: " + String(humidity) + " %, "
                 + "Pressure: " + String(pressure) + " hPa";

  // Préparer les données pour LoRa
  String jsonData = "{\"groupID\":\"" + String(groupID) + "\","
                    "\"data\":\"" + data + "\"}";

  // Serialisation et envoi des données via LoRa
  Serial.println("Envoi des données : ");
  Serial.println(jsonData);

  LoRa.beginPacket();
  LoRa.print(jsonData);
  LoRa.endPacket();

  delay(1000);
}