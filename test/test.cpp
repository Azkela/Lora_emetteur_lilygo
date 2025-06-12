#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <I2S.h>
#include <ArduinoJson.h>
#include <Base64.h>  // Ajouter la librairie Base64 pour encoder les données audio

// Configuration des pins pour LoRa
#define SS 18
#define RST 14
#define DI0 26

// Configuration des pins pour INMP441
#define I2S_DATA_PIN 35
#define I2S_BCLK_PIN 13
#define I2S_LRCLK_PIN 25


const char* groupID = "MAR-2";  // ID du groupe sécurisé

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Lora Sender + INMP441");

    // Initialisation LoRa
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(868E6)) {
        Serial.println("Erreur de démarrage LoRa !");
        while (1);
    }

    // Initialisation du microphone INMP441 (I2S)
    I2S.begin(I2S_PHILIPS_MODE, 16000, 16);  // Mode Philips, fréquence d'échantillonnage à 16 kHz

    // Augmenter la puissance d'émission LoRa
    LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
    Serial.println("LoRa et Capteurs Initialisés.");
}

void loop() {
    // Buffer pour recevoir les données audio
    int32_t audioBuffer[512];
    int bytesRead = 0;

    // Lire les données du microphone INMP441
    bytesRead = I2S.read((char*)audioBuffer, sizeof(audioBuffer));

    // Vérifier que des données sont lues
    if (bytesRead > 0) {
        // Afficher les 10 premiers échantillons pour vérifier que les données changent
        Serial.print("Audio samples: ");
        for (int i = 0; i < 10; i++) {
            Serial.print(audioBuffer[i]);
            Serial.print(" ");
        }
        Serial.println();

        // Convertir les données audio en une chaîne base64
        String audioString = "";
        for (int i = 0; i < 512; i++) {
            // Convertir chaque échantillon en une chaîne de caractères
            audioString += String(audioBuffer[i]);
            audioString += ",";
        }

        // Convertir la chaîne audio en base64
        String base64Audio = base64::encode(audioString);

        // Préparer les données pour LoRa
        StaticJsonDocument<512> doc;
        doc["groupID"] = groupID; // ID du groupe sécurisé
        doc["audio_data"] = base64Audio; // Stocke les données audio encodées en base64

        // Serialisation des données
        String jsonString;
        serializeJson(doc, jsonString);

        // Afficher les données sur le moniteur série
        Serial.print("Envoi : ");
        Serial.println(jsonString);

        // Envoi des données via LoRa
        LoRa.beginPacket();
        LoRa.print(jsonString);
        LoRa.endPacket();
    } else {
        Serial.println("Aucune donnée lue !");
    }

    delay(1000); // Attente de 1 seconde avant la prochaine mesure
}
