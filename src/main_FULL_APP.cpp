#include <Arduino.h>

// On inclut les "outils" de notre bibliothèque personnelle
#include <BME_Sensor.h>
#include <INMP441_Mic.h>
#include <Dust_Reader.h>
#include <LoRa_Transmitter.h>

// --- Définition des Pins et Constantes Globales ---
// BME280
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
// INMP441
#define I2S_WS     15
#define I2S_SD     35
#define I2S_SCK    25
// UART Mega
#define MEGA_RX_PIN 14
#define MEGA_TX_PIN 13
// LoRa
#define LORA_SS    18
#define LORA_RST   23
#define LORA_DIO0  26
// LoRa Config
#define LORA_FREQ  868E6
const char* GROUP_ID = "MAR-2";
// Timer
const long LORA_INTERVAL = 1000; // 1 secondes
unsigned long previousMillis = 0;

// --- Création de nos objets (nos "outils") ---
BME_Sensor      sensorBME;
INMP441_Mic     sensorMic;
Dust_Reader     sensorDust;
LoRa_Transmitter transmitterLoRa;

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Initialisation du chef d'orchestre ---");

  if (!sensorBME.begin(I2C_SDA_PIN, I2C_SCL_PIN)) {
    Serial.println("Échec init BME280");
  } else {
    Serial.println("BME280 OK");
  }

  if (!sensorMic.begin(I2S_WS, I2S_SD, I2S_SCK)) {
    Serial.println("Échec init Micro INMP441");
  } else {
    Serial.println("Micro INMP441 OK");
  }
  
  sensorDust.begin(MEGA_RX_PIN, MEGA_TX_PIN);
  Serial.println("Écoute du capteur de poussière OK");

  if (!transmitterLoRa.begin(LORA_FREQ, LORA_SS, LORA_RST, LORA_DIO0)) {
    Serial.println("Échec init LoRa");
    while(true);
  } else {
    Serial.println("LoRa OK");
  }
  
  Serial.println("\n--- Tous les systèmes sont prêts. ---");
}

void loop() {
  // En permanence, on vérifie si le Mega a envoyé des données
  //if (sensorDust.checkForData()) {
  //  Serial.print("Nouvelle donnée de poussière reçue : ");
  //  Serial.println(sensorDust.getDustVoltage());
  //}
  sensorDust.checkForData();

  // Toutes les 1 secondes, on lit les capteurs locaux et on envoie tout
  if (millis() - previousMillis >= LORA_INTERVAL) {
    previousMillis = millis();

    Serial.println("\n--- Cycle de lecture et d'envoi ---");
    
    sensorBME.read();
    sensorMic.read();

    transmitterLoRa.sendData(
      GROUP_ID,
      sensorBME.getTemperature(),
      sensorBME.getHumidity(),
      sensorBME.getPressure(),
      sensorMic.getSoundLevelDb(),
      sensorDust.getDustVoltage()
    );
  }
}